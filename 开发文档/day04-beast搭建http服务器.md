## 绑定和监听连接
我们利用visual studio创建一个空项目，项目名字为GateServer，然后按照day03的方法配置boost库和jsoncpp配置好后，我们添加一个新的类，名字叫CServer。添加成功后生成的CServer.h和CServer.cpp也会自动加入到项目中。

CServer类构造函数接受一个端口号，创建acceptor接受新到来的链接。

CServer.h包含必要的头文件，以及简化作用域声明
``` cpp
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
```
CServer.h中声明acceptor, 以及用于事件循环的上下文iocontext,和构造函数
``` cpp
class CServer
{
public:
	CServer(unsigned short& port);
private:
	tcp::acceptor  _acceptor;
	net::io_context _ioc;
};
```
cpp中实现构造函数如下
``` cpp
CServer::CServer(unsigned short& port) :_ioc(1),
_acceptor(_ioc, { net::ip::make_address("0.0.0.0"), port }) {
}
```
接下来我们实现Listen函数，用来监听新链接
``` cpp
void CServer::Listen()
{
	tcp::socket socket{ _ioc };
	_acceptor.async_accept(socket, [&](beast::error_code ec) {
		try {
			//出错则放弃这个连接，继续监听新链接
			if (ec) {
				Listen();
				return;
			}

			//处理新链接，创建HpptConnection类管理新连接

			//继续监听
			Listen();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
		}
	});
}
```
上面函数有个很严重的问题就是lambda表达式捕获的是局部变量的引用，而局部变量是会被释放的，所以要保证回调函数触发之前socket不会被释放。我的思路是构造HttpConnection类型的智能指针管理这个socket。

我们创建const.h将文件件和一些作用于声明放在const.h里，这样以后创建的文件包含这个const.h即可，不用写那么多头文件了。

``` cpp
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
```

新建HttpConnection类文件，在头文件添加声明
``` cpp
#include "const.h"
class HttpConnection
{
public:
	HttpConnection(net::io_context& ioc);
	tcp::socket& GetSocket();
private:
	tcp::socket  _socket;
};
```
实现

``` cpp
HttpConnection::HttpConnection(net::io_context& ioc):_socket(ioc)
{

}

tcp::socket& HttpConnection::GetSocket()
{
	return _socket;
}
```
把之前Server的Listen函数修改，捕获HttpConnection智能指针，这样能达到闭包的效果
``` cpp
void CServer::Listen()
{
	auto http_con_ptr = std::make_shared<HttpConnection>(_ioc);
	_acceptor.async_accept(http_con_ptr->GetSocket(), [http_con_ptr, this](beast::error_code ec) {
		try {
			//出错则放弃这个连接，继续监听新链接
			if (ec) {
				Listen();
				return;
			}

			//处理新链接，创建HpptConnection类管理新连接
			http_con_ptr->Start();
			//继续监听
			Listen();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
		}
	});
}
```
我们考虑在Start内部调用http::async_read函数，其源码为
``` cpp
async_read(
    AsyncReadStream& stream,
    DynamicBuffer& buffer,
    basic_parser<isRequest>& parser,
    ReadHandler&& handler)
```
第一个参数为异步可读的数据流，大家可以理解为socket.

第二个参数为一个buffer，用来存储接受的数据，因为http可接受文本，图像，音频等多种资源文件，所以是Dynamic动态类型的buffer。

第三个参数是请求参数，我们一般也要传递能接受多种资源类型的请求参数。

第四个参数为回调函数，接受成功或者失败，都会触发回调函数，我们用lambda表达式就可以了。

我们将1,2,3这几个参数写到HttpConnection类的成员声明里
``` cpp
// The buffer for performing reads.
beast::flat_buffer _buffer{ 8192 };

// The request message.
http::request<http::dynamic_body> _request;

// The response message.
http::response<http::dynamic_body> _response;

// The timer for putting a deadline on connection processing.
net::steady_timer deadline_{
	_socket.get_executor(), std::chrono::seconds(60) };
```
_buffer 用来接受数据

_request 用来解析请求

_response 用来回应客户端

_deadline 用来做定时器判断请求是否超时

HttpConnection的Start
``` cpp
void HttpConnection::Start()
{
	http::async_read(_socket, _buffer, _request, [this](beast::error_code ec,
		std::size_t bytes_transferred) {
			try {
				if (ec) {
					std::cout << "http read err is " << ec.what() << std::endl;
					return;
				}

				//处理读到的数据
				
			}
			catch (std::exception& exp) {
				std::cout << "exception is " << exp.what() << std::endl;
			}
		}
	);
}
```
如果lambda回调之前HttpConnection被回收了，那么回调函数就会访问无效的内存地址，所以我们还要对HttpConnection保活，在回调之前HttpConnection是有效的，所以要用`shared_from_this()`基于this对象生成共享的智能指针，该智能指针和之前管理this的智能指针共享引用计数，而不是我们根据this构造一个新的智能指针(会导致this被两个智能指针管理)

HttpConnection声明时继承std::enable_shared_from_this
``` cpp
class HttpConnection: public std::enable_shared_from_this<HttpConnection>{
    //...
}
```
把Start函数修改如下
``` cpp
void HttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec,
		std::size_t bytes_transferred) {
			try {
				if (ec) {
					std::cout << "http read err is " << ec.what() << std::endl;
					return;
				}

				//处理读到的数据
				boost::ignore_unused(bytes_transferred);
				self->HandleReq();
			}
			catch (std::exception& exp) {
				std::cout << "exception is " << exp.what() << std::endl;
			}
		}
	);
}
```

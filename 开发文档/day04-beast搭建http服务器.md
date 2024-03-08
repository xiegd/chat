## CServer类设计
我们利用visual studio创建一个空项目，项目名字为GateServer，然后按照day03的方法配置boost库和jsoncpp配置好后，我们添加一个新的类，名字叫CServer。添加成功后生成的CServer.h和CServer.cpp也会自动加入到项目中。

CServer类构造函数接受一个端口号，创建acceptor接受新到来的链接。

包含必要的头文件，以及简化作用域声明
``` cpp
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
```


## 注册Post请求
我们实现RegPost函数
``` cpp
void LogicSystem::RegPost(std::string url, HttpHandler handler) {
	_post_handlers.insert(make_pair(url, handler));
}
```
在const.h中添加ErrorCodes定义并且包含JsonCpp相关的头文件
``` cpp
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  //Json解析错误
	RPCFailed = 1002,  //RPC请求错误
};
```
然后在LogicSystem的构造函数里添加获取验证码的处理逻辑，
``` cpp
RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
	auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
	std::cout << "receive body is " << body_str << std::endl;
	connection->_response.set(http::field::content_type, "text/json");
	Json::Value root;
	Json::Reader reader;
	Json::Value src_root;
	bool parse_success = reader.parse(body_str, src_root);
	if (!parse_success) {
		std::cout << "Failed to parse JSON data!" << std::endl;
		root["error"] = ErrorCodes::Error_Json;
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;
	}

	auto email = src_root["email"].asString();
	cout << "email is " << email << endl;
	root["error"] = 0;
	root["email"] = src_root["email"];
	std::string jsonstr = root.toStyledString();
	beast::ostream(connection->_response.body()) << jsonstr;
	return true;
});
```
然后我们在LogicSystem中添加Post请求的处理
``` cpp
bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con) {
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}

	_post_handlers[path](con);
	return true;
}
```
在HttpConnection的HandleReq中添加post请求处理
``` cpp
void HttpConnection::HandleReq() {
    //省略...
	if (_request.method() == http::verb::post) {
		bool success = LogicSystem::GetInstance()->HandlePost(_request.target(), shared_from_this());
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}

		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}

}
```
然后我们启动服务器，然后下载postman，大家可以去官网下载，如果速度慢可以去我的网盘下载
[https://pan.baidu.com/s/1DBIf7Y6G3v0XYfW5LyDKMg?pwd=kjxz](https://pan.baidu.com/s/1DBIf7Y6G3v0XYfW5LyDKMg?pwd=kjxz)

提取码：kjxz 

打开postman，将请求修改为post

![https://cdn.llfc.club/1710163652661.png](https://cdn.llfc.club/1710163652661.png)

绿色的为post请求的json参数，红色的为服务器返回的json数据包。

我们看服务器打印的日志

![https://cdn.llfc.club/1710164199348.jpg](https://cdn.llfc.club/1710164199348.jpg)



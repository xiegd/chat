#pragma once
#include "const.h"
class HttpConnection: public std::enable_shared_from_this<HttpConnection>
{
public:
	HttpConnection(net::io_context& ioc);
	tcp::socket& GetSocket();
	void Start();
private:
	tcp::socket  _socket;
	// The buffer for performing reads.
	beast::flat_buffer  _buffer{ 8192 };

	// The request message.
	http::request<http::dynamic_body> _request;

	// The response message.
	http::response<http::dynamic_body> _response;

	// The timer for putting a deadline on connection processing.
	net::steady_timer deadline_{
		_socket.get_executor(), std::chrono::seconds(60) };
};


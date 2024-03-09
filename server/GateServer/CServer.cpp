#include "CServer.h"
#include <iostream>
#include "HttpConnection.h"
CServer::CServer(unsigned short& port) :_ioc(1),
_acceptor(_ioc, { net::ip::make_address("0.0.0.0"), port }) {
	
}

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
			Listen();
		}
	});
}


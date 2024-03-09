#pragma once
#include <string>
#include "const.h"

class CServer
{
public:
	CServer(unsigned short& port);
	void Listen();
private:
	tcp::acceptor  _acceptor;
	net::io_context _ioc;
};


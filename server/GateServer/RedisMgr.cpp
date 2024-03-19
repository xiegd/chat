#include "RedisMgr.h"

RedisMgr::RedisMgr() {

}

bool RedisMgr::Connect(std::string host, int port)
{
	this->_connect = redisConnect(host.c_str(), port);
	if (this->_connect != NULL && this->_connect->err)
	{
		printf("connect error: %s\n", this->_connect->errstr);
		return 0;
	}
	return 1;
}
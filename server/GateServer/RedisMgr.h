#pragma once
#include "const.h"
#include "hiredis.h"
class RedisMgr: public Singleton<RedisMgr>, 
	public std::enable_shared_from_this<RedisMgr>
{
	friend class Singleton<RedisMgr>;
public:
	bool Connect(std::string host, int port);
private:
	RedisMgr();

	redisContext* _connect;
	redisReply* _reply;
};


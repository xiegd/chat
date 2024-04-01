#pragma once
#include "const.h"
#include "MysqlDao.h"
class MysqlMgr: public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	bool GetUserId(int& id);
private:
	MysqlMgr();
	MysqlDao  _dao;
};


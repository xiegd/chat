#pragma once
#include "const.h"
#include "MysqlDao.h"
class MysqlMgr: public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	bool GetUserId(int& id);
	bool ExistUser(const std::string& name, bool & exist);
private:
	MysqlMgr();
	MysqlDao  _dao;
};


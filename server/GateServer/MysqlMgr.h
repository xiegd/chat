#pragma once
#include "const.h"
class MysqlMgr: public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
private:
	MysqlMgr();
	std::unique_ptr<sql::Connection> _con;
	std::unique_ptr<sql::PreparedStatement> _prep_stmt;
	std::unique_ptr<sql::Statement> _stmt;
};


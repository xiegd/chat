#include "MysqlMgr.h"


MysqlMgr::~MysqlMgr() {

}

int MysqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	return _dao.RegUser(name, email, pwd);
}

bool MysqlMgr::CheckValid(const std::string& name, const std::string& email) {
	return _dao.CheckValid(name, email);
}

MysqlMgr::MysqlMgr() {
}




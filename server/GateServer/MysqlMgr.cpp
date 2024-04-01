#include "MysqlMgr.h"


MysqlMgr::~MysqlMgr() {

}

MysqlMgr::MysqlMgr() {
}

bool MysqlMgr::GetUserId(int& id){
	return _dao.GetUserId(id);
}

bool MysqlMgr::ExistUser(const std::string& name, bool& exist)
{
	return _dao.ExistUser(name, exist);
}

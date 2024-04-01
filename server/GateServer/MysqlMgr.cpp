#include "MysqlMgr.h"


MysqlMgr::~MysqlMgr() {

}

MysqlMgr::MysqlMgr() {
}

bool MysqlMgr::GetUserId(int& id){
	return _dao.GetUserId(id);
}

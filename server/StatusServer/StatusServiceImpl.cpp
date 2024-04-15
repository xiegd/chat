#include "StatusServiceImpl.h"
#include "ConfigMgr.h"

Status StatusServiceImpl::GetChatServerIP(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
	std::string prefix("llfc status server has received :  ");
	reply->set_serverip("127.0.0.1:8090");
	reply->set_token("zack1024");
	return Status::OK;
}

StatusServiceImpl::StatusServiceImpl()
{
	//_servers
}

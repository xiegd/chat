#include "ChatServiceImpl.h"
#include "UserMgr.h"
#include "CSession.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

ChatServiceImpl::ChatServiceImpl()
{

}

Status ChatServiceImpl::NotifyAddFriend(ServerContext* context, const AddFriendReq* request, AddFriendRsp* reply)
{
	//查找用户是否在本服务器
	auto touid = request->touid();
	auto session = UserMgr::GetInstance()->GetSession(touid);
	//用户不在内存中则直接返回，将申请写入数据库即可
	if (session == nullptr) {
		return Status::OK;
	}

	Json::Value  rtvalue;
	rtvalue["error"] = ErrorCodes::Success;
	rtvalue["applyuid"] = request->applyuid();
	rtvalue["name"] = request->name();
	rtvalue["desc"] = request->desc();

	//在内存中则将此申请写入数据库，并且转发给当前用户
	std::string return_str = rtvalue.toStyledString();
	session->Send(return_str, ID_NOTIFY_ADD_FRIEND_REQ);
		
	return Status::OK;
}

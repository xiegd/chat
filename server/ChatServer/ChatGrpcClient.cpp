#include "ChatGrpcClient.h"
#include "RedisMgr.h"
#include "ConfigMgr.h"
#include "UserMgr.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "CSession.h"

ChatGrpcClient::ChatGrpcClient()
{
	auto& cfg = ConfigMgr::Inst();
	auto server_list = cfg["PeerServer"]["Servers"];

	std::vector<std::string> words;

	std::stringstream ss(server_list);
	std::string word;

	while (std::getline(ss, word, ',')) {
		words.push_back(word);
	}

	for (auto& word : words) {
		if (cfg[word]["Name"].empty()) {
			continue;
		}
		_pools[cfg[word]["Name"]] = std::make_unique<ChatConPool>(5, cfg[word]["Host"], cfg[word]["Port"]);
	}

}

AddFriendRsp ChatGrpcClient::NotifyAddFriend(std::string server_ip, const AddFriendReq& req)
{
	AddFriendRsp rsp;
	Defer defer([&rsp, &req]() {
		rsp.set_error(ErrorCodes::Success);
		rsp.set_applyuid(req.applyuid());
		rsp.set_touid(req.touid());
		});

	auto find_iter = _pools.find(server_ip);
	if (find_iter == _pools.end()) {
		return rsp;
	}
	
	auto &cfg = ConfigMgr::Inst();
	auto self_name = cfg["SelfServer"]["Name"];
	//直接通知对方有申请消息
	if (server_ip == self_name) {
		auto session = UserMgr::GetInstance()->GetSession(req.touid());
		if (session) {
			//在内存中则直接发送通知对方
			Json::Value  rtvalue;
			rtvalue["error"] = ErrorCodes::Success;
			rtvalue["applyuid"] = req.applyuid();
			rtvalue["name"] = req.name();
			rtvalue["desc"] = req.desc();
			std::string return_str = rtvalue.toStyledString();
			session->Send(return_str, ID_NOTIFY_ADD_FRIEND_REQ);
		}

		return rsp;
	}

	auto &pool = find_iter->second;
	ClientContext context;
	auto stub = pool->getConnection();
	Status status = stub->NotifyAddFriend(&context, req, &rsp);
	Defer defercon([&stub, this, &pool]() {
		pool->returnConnection(std::move(stub));
		});

	if (!status.ok()) {
		rsp.set_error(ErrorCodes::RPCFailed);
		return rsp;
	}

	return rsp;
}

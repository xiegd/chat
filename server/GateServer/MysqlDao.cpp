#include "MysqlDao.h"
#include "ConfigMgr.h"
MysqlDao::MysqlDao()
{
	auto & cfg = ConfigMgr::Inst();
	const auto& host = cfg["Mysql"]["Host"];
	const auto& port = cfg["Mysql"]["Port"];
	const auto& pwd = cfg["Mysql"]["Passwd"];
	const auto& schema = cfg["Mysql"]["Schema"];
	const auto& user = cfg["Mysql"]["User"];
	pool_.reset(new MySqlPool(host+":"+port, user, pwd,schema, 5));
}

MysqlDao::~MysqlDao(){
	pool_->Close();
}

bool MysqlDao::GetUserId(int& id)
{
	auto con = pool_->getConnection();
	try {	
		if (con == nullptr) {
			pool_->returnConnection(std::move(con));
			return false;
		}

		std::unique_ptr<sql::Statement> stmt(con->createStatement());
		// 执行存储过程
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("CALL get_user_id()"));

		// 处理结果集
		while (res->next()) {
			id = res->getInt("id");
			std::cout << "New ID: " << id << std::endl;
			break;
		}

		pool_->returnConnection(std::move(con));
		return true;
	}
	catch (sql::SQLException& e) {
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::ExistUser(const std::string& name, bool& exist)
{
	//auto con = pool_->getConnection();
	//try {
	//	if (con == nullptr) {
	//		pool_->returnConnection(std::move(con));
	//		return false;
	//	}

	//	std::unique_ptr<sql::Statement> stmt(con->createStatement());
	//	// 执行存储过程
	//	std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("CALL get_user_id()"));

	//	// 处理结果集
	//	while (res->next()) {
	//		id = res->getInt("id");
	//		std::cout << "New ID: " << id << std::endl;
	//		break;
	//	}

	//	pool_->returnConnection(std::move(con));
	//	return true;
	//}
	//catch (sql::SQLException& e) {
	//	pool_->returnConnection(std::move(con));
	//	std::cerr << "SQLException: " << e.what();
	//	std::cerr << " (MySQL error code: " << e.getErrorCode();
	//	std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	//	return false;
	//}
}



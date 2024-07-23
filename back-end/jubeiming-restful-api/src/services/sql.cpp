// SQL实现

#include "common/utils.hpp"
#include "boost/mysql.hpp"
#include "sql.hpp"
#include "fmt/core.h"

using namespace jubeiming::service;
using namespace jubeiming;

std::vector<define::sql::user> sql::ZhenNiu::getUser(const std::string& condition, const std::string& arg)
{
	boost::mysql::statement stmt{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("SELECT * FROM Users WHERE UserUUID = ? ;") };
	boost::mysql::static_results<define::sql::user> result;
	mConn.getDB("ZhenNiuAppSystem")->execute(stmt.bind(arg), result);
	std::vector<define::sql::user> users;
	for (const auto& u : result.rows())
	{
		users.push_back(u);
	}
	return users;
}

std::vector<define::sql::user> sql::ZhenNiu::getUser(const std::string& condition, int64_t arg)
{
	boost::mysql::statement stmt{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("SELECT * FROM Users WHERE UserUUID = ? ;")};
	boost::mysql::static_results<define::sql::user> result;
	mConn.getDB("ZhenNiuAppSystem")->execute(stmt.bind(arg), result);
	std::vector<define::sql::user> users;
	for (const auto& u : result.rows())
	{
		users.push_back(u);
	}
	return users;
}

sql::ZhenNiu::ZhenNiu(utils::MySQLConnector& conn):mConn { conn } {}

std::vector<define::sql::user> sql::ZhenNiu::getUserByID(std::int64_t id){
	return getUser("UserID", id);
}

std::vector<define::sql::user> sql::ZhenNiu::getUserByUUID(const std::string& uuid)
{
	return getUser("UserUUID", uuid);
}

std::vector<define::sql::user> sql::ZhenNiu::getUserByName(const std::string& username)
{
	return getUser("UserName", username);
}

std::vector<define::sql::user> jubeiming::service::sql::ZhenNiu::getWeiUserByUUID(const std::string& uuid)
{
	boost::mysql::statement stmt{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("SELECT * FROM WXAppQuery WHERE UserUUID = ?;") };
	boost::mysql::results result;
	mConn.getDB("ZhenNiuAppSystem")->execute(stmt.bind(uuid), result);

	for (const auto& value : result.rows())
	{
		value.at(0).as_int64(); // WeiID
		value.at(1).as_string(); // OpenID
		if (not value.at(2).is_null()) value.at(2).as_string(); // UnionID
		value.at(3).as_string(); // Nickname
		value.at(4).as_int64(); // Gender
		if (not value.at(5).is_null()) value.at(5).as_string(); // AvatarUrl
		value.at(6).as_string(); // UserUUID

		value.at(7).as_int64(); // UserID
		if (not value.at(8).is_null()) value.at(8).as_string(); // PasswordHash
		if (not value.at(9).is_null()) value.at(9).as_string(); // UserName
		if (not value.at(10).is_null()) value.at(10).as_string(); // PhoneNumber
		if (not value.at(11).is_null()) value.at(11).as_string(); // Email
		if (not value.at(12).is_null()) value.at(12).as_datetime(); // RegistrationDate
		if (not value.at(13).is_null()) value.at(13).as_int64(); // UserStatus
		if (not value.at(14).is_null()) value.at(14).as_int64(); // UserLevel
		if (not value.at(15).is_null()) value.at(15).as_int64(); // UserGrant
		if (not value.at(16).is_null()) value.at(16).as_datetime(); // Scope
	}
	// WXAppQuery
	return std::vector<define::sql::user>();
}

std::optional<define::sql::user> jubeiming::service::sql::ZhenNiu::getUserByPhoneNumber(const std::string& phoneNumber)
{
	boost::mysql::statement stmt{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("SELECT * FROM Users WHERE PhoneNumber = ? ;") };
	boost::mysql::static_results<define::sql::user> result;
	mConn.getDB("ZhenNiuAppSystem")->execute(stmt.bind(phoneNumber), result);
	std::vector<define::sql::user> users;
	for (const auto& u : result.rows())
	{
		users.push_back(u);
	}

	std::optional<define::sql::user> user{
		[&]() -> std::optional<define::sql::user> {
			if (not users.empty()) {
				return users.at(0);
			}
			return std::nullopt;
		}()
	};

	return user;
}

std::optional<define::sql::WeiApp> jubeiming::service::sql::ZhenNiu::getWeiAppInfoByOpenID(const std::string& openid)
{
	boost::mysql::statement stmt{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("SELECT * FROM WeiAPP WHERE OpenID = ? ;") };
	boost::mysql::static_results<define::sql::WeiApp> result;
	mConn.getDB("ZhenNiuAppSystem")->execute(stmt.bind(openid), result);
	std::vector<define::sql::WeiApp> weiApps;
	for (const auto& u : result.rows())
	{
		weiApps.push_back(u);
	}

	std::optional<define::sql::WeiApp> weiApp{
		[&]() -> std::optional<define::sql::WeiApp> {
			if (not weiApps.empty()) {
				return weiApps.at(0);
			}
			return std::nullopt;
		}()
	};

	return weiApp;
}

std::optional<define::sql::WeiApp> jubeiming::service::sql::ZhenNiu::getWeiAppInfoByUUID(const std::string& uuid)
{
	boost::mysql::statement stmt{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("SELECT * FROM WeiAPP WHERE UserUUID = ? ;") };
	boost::mysql::static_results<define::sql::WeiApp> result;
	mConn.getDB("ZhenNiuAppSystem")->execute(stmt.bind(uuid), result);
	std::vector<define::sql::WeiApp> weiApps;
	for (const auto& u : result.rows())
	{
		weiApps.push_back(u);
	}

	std::optional<define::sql::WeiApp> weiApp{
		[&]() -> std::optional<define::sql::WeiApp> {
			if (not weiApps.empty()) {
				return weiApps.at(0);
			}
			return std::nullopt;
		}()
	};

	return weiApp;
}

void jubeiming::service::sql::ZhenNiu::addNewWeiChatAPPAccount(const define::sql::WeiApp& weiApp, const define::sql::user& user)
{
	boost::mysql::results result;
	
	std::string uuid, weiUserUUID{ weiApp.UserUUID }, UserUUID{ user.UserUUID };

	bool has_create{ true };

	if (user.PhoneNumber.has_value()) {
		auto userdb{ getUserByPhoneNumber(user.PhoneNumber.value()) };
		if (userdb.has_value()) {
			UserUUID = weiUserUUID = userdb.value().UserUUID;
			has_create = false;
		}
	}

	boost::mysql::statement weiAppStat{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("INSERT INTO WeiAPP(UserUUID, OpenID, UnionID) VALUES(?, ?, ?);") };
	mConn.getDB("ZhenNiuAppSystem")->execute(weiAppStat.bind(weiUserUUID, weiApp.OpenID, weiApp.UnionID), result);
	
	if (has_create) {
		boost::mysql::statement uersStat{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("INSERT INTO Users (UserUUID, PhoneNumber, UserStatus, UserLevel, UserGrant) VALUES(?, ?, 1, 0, 1);") };
		mConn.getDB("ZhenNiuAppSystem")->execute(uersStat.bind(UserUUID, user.PhoneNumber), result);
	}
}

void jubeiming::service::sql::ZhenNiu::addNewAccount(const define::sql::user& user){
	boost::mysql::results result;
	boost::mysql::statement uersStat{ mConn.getDB("ZhenNiuAppSystem")->prepare_statement("INSERT INTO Users (UserUUID, PhoneNumber, UserStatus, UserLevel, UserGrant) VALUES(?, ?, 1, 0, 1);") };
	mConn.getDB("ZhenNiuAppSystem")->execute(uersStat.bind(user.UserUUID, user.PhoneNumber), result);
}

std::string jubeiming::service::sql::ZhenNiu::modifyUserRecordSQL(std::string_view condition, std::string_view setSQL)
{
	std::string sql { fmt::format("UPDATE Users SET {} WHERE {};", setSQL, condition) };
	return sql;
}

void jubeiming::service::sql::ZhenNiu::modifyUserLevel(std::string_view level, std::string& uuid)
{
	utils::MySQLReplaceDangerousString(uuid);
	auto sql{ modifyUserRecordSQL(level, fmt::format("UserUUID = '{}'", uuid))};
	
	boost::mysql::results result;
	mConn.getDB("ZhenNiuAppSystem")->execute(sql, result);
}

std::vector<define::sql::client> jubeiming::service::sql::Auth::getClient(const std::string& arg)
{
	boost::mysql::statement stmt{ mConn.getDB("Auth")->prepare_statement("SELECT * FROM client WHERE ClientUUID = ? ;") };
	boost::mysql::static_results<define::sql::client> result;
	mConn.getDB("Auth")->execute(stmt.bind(arg), result);
	std::vector<define::sql::client> clients;
	for (const auto& client : result.rows())
	{
		clients.push_back(client);
	}
	return clients;
}

std::vector<define::sql::client> jubeiming::service::sql::Auth::getClient(std::int64_t arg)
{
	boost::mysql::statement stmt{ mConn.getDB("Auth")->prepare_statement("SELECT * FROM client WHERE ClientID = ? ;") };
	boost::mysql::static_results<define::sql::client> result;
	mConn.getDB("Auth")->execute(stmt.bind(arg), result);
	std::vector<define::sql::client> clients;
	for (const auto& client : result.rows())
	{
		clients.push_back(client);
	}
	return clients;
}

jubeiming::service::sql::Auth::Auth(utils::MySQLConnector& conn) :mConn{ conn } {}

std::vector<define::sql::client> jubeiming::service::sql::Auth::getClientByID(std::int64_t id)
{
	return getClient(id);
}

std::vector<define::sql::client> jubeiming::service::sql::Auth::getClientByUUID(const std::string& uuid)
{
	return getClient(uuid);
}
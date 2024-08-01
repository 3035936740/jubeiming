/*
 * @File	  : sql.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : SQL定义
*/

#pragma once

#include <memory>
#include <vector>
#include <optional>
#include "define.hpp"
#include "common/mysql_conn.hpp"
#include "boost/mysql/tcp.hpp"

#ifndef JUBEIMING_SQL_HPP
#define JUBEIMING_SQL_HPP

namespace jubeiming::service::sql {
	using namespace jubeiming;
	class ZhenNiu {
	private:
		utils::MySQLConnector& mConn;
		std::vector<define::sql::user> getUser(const std::string&, const std::string&);
		std::vector<define::sql::user> getUser(const std::string&, std::int64_t);
	public:
		ZhenNiu(utils::MySQLConnector& conn);
		std::vector<define::sql::user> getUserByID(std::int64_t);
		std::vector<define::sql::user> getUserByUUID(const std::string&);
		std::vector<define::sql::user> getUserByName(const std::string&);

		std::vector<define::sql::user> getWeiUserByUUID(const std::string&);

		std::optional<define::sql::user> getUserByPhoneNumber(const std::string&);
		std::optional<define::sql::WeiApp> getWeiAppInfoByOpenID(const std::string&);
		std::optional<define::sql::WeiApp> getWeiAppInfoByUUID(const std::string&);

		void addNewWeiChatAPPAccount(const define::sql::WeiApp&, const define::sql::user&);
		void addNewAccount(const define::sql::user&);

		std::string modifyUserRecordSQL(std::string_view, std::string_view);
		void modifyUserLevel(std::string_view, std::string&);
	};

	class Auth {
	private:
		utils::MySQLConnector& mConn;

		std::vector<define::sql::client> getClient(const std::string&);
		std::vector<define::sql::client> getClient(std::int64_t);
	public:
		Auth(utils::MySQLConnector& conn);
		std::vector<define::sql::client> getClientByID(std::int64_t);
		std::vector<define::sql::client> getClientByUUID(const std::string&);
	};

    class WebApp {
    private:
        utils::MySQLConnector& mConn;
    public:
        WebApp(utils::MySQLConnector& conn);

        std::vector<define::sql::WebApp> getWebAppByUUID(const std::string&);
        std::vector<define::sql::WebApp> addWebApp(const std::string&);
    };
}

#endif // JUBEIMING_SQL_HPP
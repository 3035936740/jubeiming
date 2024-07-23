/*
 * @File	  : AuthRouter.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : auth»œ÷§
*/

#pragma once

#ifndef JUBEIMING_AUTH_HPP
#define JUBEIMING_AUTH_HPP

#include "define.hpp"
#include "IRouter.hpp"
#include "common/mysql_conn.hpp"
#include "common/log.hpp"
#include "nlohmann/json.hpp"
#include "sw/redis++/redis++.h"

namespace jubeiming::router {
	class AuthRouter : public IRouter {
	private:
		std::unordered_map<std::string, define::sql::client> mClient;
		std::shared_ptr<sw::redis::Redis> mRedis;
		define::WeiChatApp mWeiChatAppInfo;
		utils::MySQLConnector& mConn;
		std::shared_ptr<jubeiming::Log> mLogger;
		const jubeiming::define::UuidInfo& mUuidInfo;
		const jubeiming::define::TokenInfo& mTokenInfo;
		const jubeiming::define::PhoneVerify& mPhoneVerify;
		virtual void VerifyCodeMethod(const std::string&, const std::string&, const std::string&, const std::string&);
	public:
		virtual std::string createRefreshToken(const std::string&);
		virtual std::string createAccessToken(const std::string&);
		virtual std::string getAccessTokenWarpper(const std::string&);
		virtual nlohmann::json getAccessToken(std::string_view);
		virtual nlohmann::json getRefreshToken(std::string_view);
	public:
		AuthRouter(define::CrowApp, utils::MySQLConnector&, const jubeiming::define::UuidInfo&, std::shared_ptr<sw::redis::Redis>, const jubeiming::define::TokenInfo&, const jubeiming::define::PhoneVerify&);
		
		virtual ~AuthRouter() = default;

		virtual void setClient(std::string_view, define::sql::client&&);
		virtual void setWeiChatAPPInfo(define::WeiChatApp&&);
		virtual void setLogger(std::shared_ptr<jubeiming::Log>);
		virtual void startRouter();
	};
}

#endif // JUBEIMING_AUTH_HPP
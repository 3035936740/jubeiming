/*
 * @File	  : define.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2024/06/25 15:47
 * @Introduce : 定义
*/

#pragma once

#include <memory>
#include <string>
#include <optional>
#include <chrono>
#include "boost/describe.hpp"
#include "boost/mysql/datetime.hpp"
#include "boost/mysql/tcp.hpp"
#include "crow/app.h"

#ifndef JUBEIMING_DEFINE_HPP
#define JUBEIMING_DEFINE_HPP

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace jubeiming::define {
	struct WeiChatApp {
		std::string appid;
		std::string secret;
		std::string url;
	};

	struct UuidInfo {
		std::string uuidNamespace;
		std::int64_t snowflakeTwepoch;
		std::int64_t snowflakeWorkerid;
		std::int64_t snowflakeDatacenterid;
	};

	struct TokenInfo {
		std::time_t refreshExpire;
		std::time_t accessExpire;
	};

	struct PhoneVerify {
		std::string url;
		std::string authorization;
		std::time_t codeValidity;
	};

	namespace sql {
		// https://www.boost.org/doc/libs/1_85_0/libs/mysql/doc/html/mysql/dynamic_interface.html#mysql.dynamic_interface.mysql_to_c_type_mapping_referenc
		// https://www.boost.org/doc/libs/1_85_0/libs/mysql/doc/html/mysql/dynamic_interface.html#mysql.dynamic_interface.mysql_to_c_type_mappings

		struct user {
			std::uint64_t UserID; // 用户id
			std::string UserUUID; // 用户uuid
			std::optional<std::string> UserName; // 用户名
			std::optional<std::string> PasswordHash; // 用户密码的hash
			std::optional<std::string> PhoneNumber; // 用户手机号
			std::optional<std::string> Email; // 用户邮箱
			std::optional<boost::mysql::datetime> RegistrationDate; // 用户注册日期
			std::optional<std::int64_t> UserStatus; // 用户状态(0 封禁, 1 正常, 2 VIP1, 3 VIP2, 4 VIP3, 5 VIP4, ...)
			std::optional<std::int64_t> UserLevel; // 用户等级(分发等级)
			std::optional<std::int64_t> UserGrant; // 用户权限

			/* *
			read 读取服务器数据
			write 上传至服务器
			comment 评论
			admin 管理员
			* */
			std::optional<std::string> Scope; // Scope
		};

		struct WeiApp {
			std::uint64_t ID; // 用户id
			std::string UserUUID; // 用户uuid
			std::string OpenID; // 用户密码的hash
			std::optional<std::string> UnionID; // 用户名
			std::string Nickname; // 用户名
			std::int64_t Gender; // 性别
			std::optional<std::string> AvatarUrl; // 小程序头像
		};

        struct WebApp {
			std::uint64_t ID; // 用户id
			std::string UserUUID; // 用户uuid
            std::optional<std::string> AvatarUrl; // 用户头像url
		};

		BOOST_DESCRIBE_STRUCT(define::sql::user, (), (UserID, UserUUID, UserName, PasswordHash, PhoneNumber, Email, RegistrationDate, UserStatus, UserLevel, UserGrant, Scope))
		BOOST_DESCRIBE_STRUCT(define::sql::WeiApp, (), (ID, UserUUID, OpenID, UnionID, Nickname, Gender, AvatarUrl))
		BOOST_DESCRIBE_STRUCT(define::sql::WebApp, (), (ID, UserUUID, AvatarUrl))

		struct client {
			std::uint64_t ClientID; // id
			std::string ClientUUID; // uuid
			std::string ClientSecret; // 用户密码的hash
			std::string Name; // 用户名
			std::optional<std::string> Scope; // 作用域
			boost::mysql::datetime CreatedAt; // 创建日期
			boost::mysql::datetime UpdatedAt; // 更新日期
		};

		BOOST_DESCRIBE_STRUCT(define::sql::client, (), (ClientID, ClientUUID, ClientSecret, Name, Scope, CreatedAt, UpdatedAt))
	};

	namespace code {
		enum status {
			UnknownError = -10000,
			PhoneInconsistentError = -10001,
			TokenExpireError = -10002,
			TokenNotExistError = -10003,
			TokenIsEmptyError = -10004,
			DataEmptyError = -10005,
			ExpireError = -10006,
			VerifyNotExistError = -10007,
			VerifyCodeIncorrect = -10008,
			NotSupport = -10009,
			Success = -20000,
			Create = -20001,
			Exist = -20002,
		};
	}

#ifdef MIDDLEWARE_ENABLED
	using CrowApplication = crow::App<>;
#else
	using CrowApplication = crow::SimpleApp;
#endif

	using CrowApp = std::shared_ptr<CrowApplication>;
}

#endif // JUBEIMING_DEFINE_HPP
/*
 * @File	  : global.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2024/06/24 16:41
 * @Introduce : 全局变量存储
*/

#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>
#include "common/log.hpp"
#include "common/sensitive_words.hpp"
#include "boost/mysql/tcp.hpp"
#include "define.hpp"
#include "sw/redis++/redis++.h"
#include  "common/mysql_conn.hpp"

#ifndef JUBEIMING_GLOBAL_HPP
#define JUBEIMING_GLOBAL_HPP  

namespace jubeiming::global {
	namespace gen {
		inline jubeiming::define::UuidInfo uuidInfo;
	}
	inline std::shared_ptr<jubeiming::Log> logger;

	namespace app{
		inline std::int32_t concurrency;
		inline std::uint16_t port;
		inline std::uint8_t timeout;
		inline std::string resourcePath;
		inline std::string host;
	}

	namespace db{
		inline boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints;
		inline utils::MySQLConnector conns;
		inline std::shared_ptr<sw::redis::Redis> redis;

	}

	namespace other {
		utils::AhoCorasick sensitiveWordsAC;
		define::WeiChatApp WeiChat;
		jubeiming::define::TokenInfo token;
		define::PhoneVerify phoneVerify;
	}

	namespace clients {
		define::sql::client WeiChatAPP;
		define::sql::client Web;
	}
}

#endif // JUBEIMING_GLOBAL_HPP  
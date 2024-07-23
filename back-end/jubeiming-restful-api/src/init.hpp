/*
 * @File	  : init.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2024/06/24 16:15
 * @Introduce : 初始化
*/

#pragma once

#include "global.hpp"
#include "yaml-cpp/yaml.h"
#include <unordered_set>
#include <chrono>

#ifndef JUBEIMING_INIT_HPP
#define JUBEIMING_INIT_HPP  

namespace {
	int __init__{ [] {
		using namespace jubeiming::global;
		using jubeiming::global::logger;
		// Yaml initialization
		const YAML::Node configYaml{ YAML::LoadFile("./config.yaml") };
		// Log
		{
			const auto& logConf{ configYaml["log"] };
			const auto loggerName{ logConf["logger-name"].as<std::string>() };
			const auto logFilename{ logConf["filename"].as<std::string>() };
			const auto hour{ logConf["hour"].as<int>() };
			const auto minute{ logConf["minute"].as<int>() };
			logger = std::make_shared<jubeiming::Log>(jubeiming::Log(loggerName, logFilename, hour, hour));
			logger->info("日志初始化完成");
		}

		// DB
		{
			const auto& dbConf{ configYaml["db"] };
			// MySQL
			{
				const auto& mysqlConf{ dbConf["mysql"] };
				auto host{ mysqlConf["host"].as<std::string>() };
				auto port{ mysqlConf["port"].as<std::string>() };
				auto user{ mysqlConf["user"].as<std::string>() };
				auto usedbs{ mysqlConf["db"].as<std::vector<std::string>>() };
				auto password{ mysqlConf["password"].as<std::string>() };
				db::conns = jubeiming::utils::MySQLConnector(host, port, user, password);

				for (const auto& usedb : usedbs) {
					db::conns.addDB(usedb);
				}
				logger->info("MySQL初始化完成");
			}

			// Redis
			{
				const auto& redisConf{ dbConf["redis"] };
				const auto host{ redisConf["host"].as<std::string>() };
				const auto port{ redisConf["port"].as<std::uint16_t>() };
				const auto user{ redisConf["user"].as<std::string>() };
				const auto password{ redisConf["password"].as<std::string>() };
				const auto db{ redisConf["db"].as<std::uint8_t>() };
				const auto socketTimeout{ redisConf["socket-timeout"].as<std::time_t>() };
				const auto connectTimeout{ redisConf["connect-timeout"].as<std::time_t>() };
				sw::redis::ConnectionOptions connectionOptions;

				connectionOptions.port = port;
				connectionOptions.host = host;
				connectionOptions.user = user;
				connectionOptions.password = password;
				connectionOptions.db = db;
				connectionOptions.socket_timeout = std::chrono::milliseconds(socketTimeout);
				connectionOptions.connect_timeout = std::chrono::milliseconds(connectTimeout);

				db::redis = std::make_shared<sw::redis::Redis>(sw::redis::Redis(connectionOptions));
				logger->info("Redis初始化完成");
			}
		}

		// Search Engine
		{
			const auto& searchEngineConf{ configYaml["search-engine"] };
			// Meilisearch
			{
				const auto& meilisearchConf{ searchEngineConf["meilisearch"] };
				const auto url{ meilisearchConf["url"].as<std::string>() };
				const auto authorization{ meilisearchConf["authorization"].as<std::string>() };
				logger->info("Meilisearch初始化完成");
			}
		}

		// Message Queue
		{
			const auto& messageQueueConf{ configYaml["mq"] };
			// Rabbitmq
			{
				const auto& rabbitmqConf{ messageQueueConf["rabbitmq"] };
				const auto host{ rabbitmqConf["host"].as<std::string>() };
				const auto port{ rabbitmqConf["port"].as<std::uint16_t>() };
				const auto user{ rabbitmqConf["user"].as<std::string>() };
				const auto password{ rabbitmqConf["password"].as<std::string>() };
				logger->info("RabbitMQ初始化完成");
			}
		}

		// Other
		{
			const auto& otherConf{ configYaml["other"] };
			std::filesystem::path sensitiveWordsPath { otherConf["sensitive-words"].as<std::string>() };
			// Load Sensitive Words
			jubeiming::utils::loadSensitiveWords(sensitiveWordsPath, other::sensitiveWordsAC);
			logger->info("敏感词过滤初始化完成");

			// WeiChat Mini APP
			{
				const auto& weiChatMiniConf{ otherConf["wei-chat-app"] };
				other::WeiChat.appid = weiChatMiniConf["appid"].as<std::string>();
				other::WeiChat.secret = weiChatMiniConf["secret"].as<std::string>();
				other::WeiChat.url = weiChatMiniConf["url"].as<std::string>();
				logger->info("微信小程序相关数据读取完成");
			}

			// Client ID取得
			{
				const auto& clients{ otherConf["clients"] };
				const auto& web{ clients["web"] };
				const auto& weichatapp{ clients["wei-chat-app"] };
				jubeiming::global::clients::Web.ClientUUID = web["uuid"].as<std::string>();
				jubeiming::global::clients::Web.ClientSecret = web["secret"].as<std::string>();
				jubeiming::global::clients::WeiChatAPP.ClientUUID = weichatapp["uuid"].as<std::string>();
				jubeiming::global::clients::WeiChatAPP.ClientSecret = weichatapp["secret"].as<std::string>();
				logger->info("client相关完成加载");
			}

			// Token时效初始化
			{
				const auto& token{ otherConf["token"] };
				jubeiming::global::other::token.refreshExpire = token["refresh-expire"].as<std::time_t>();
				jubeiming::global::other::token.accessExpire = token["access-token"].as<std::time_t>();
				logger->info("Token时效相关完成加载");
			}

			// Phone Verify
			{
				const auto& phoneApi{ otherConf["phone-api"] };
				jubeiming::global::other::phoneVerify.url = phoneApi["url"].as<std::string>();
				jubeiming::global::other::phoneVerify.authorization = phoneApi["authorization"].as<std::string>();
				jubeiming::global::other::phoneVerify.codeValidity = phoneApi["code-validity"].as<std::time_t>();
				logger->info("手机验证相关完成加载");
			}
		}

		// Server
		{
			const auto& serverConf{ configYaml["server"] };
			// Gen UUID
			{
				const auto& genConf{ serverConf["generator"] };
				const auto& uuidConf{ genConf["uuid"] };
				gen::uuidInfo.uuidNamespace = uuidConf["namespace"].as<std::string>();
				gen::uuidInfo.snowflakeTwepoch = uuidConf["twepoch"].as<std::int64_t>();
				gen::uuidInfo.snowflakeWorkerid = uuidConf["workerid"].as<std::int64_t>();
				gen::uuidInfo.snowflakeDatacenterid = uuidConf["datacenterid"].as<std::int64_t>();
				logger->info("snowflake初始化完成");
			}
			app::concurrency = serverConf["concurrency"].as<std::int32_t>();
			app::port = serverConf["port"].as<std::uint16_t>();
			app::resourcePath = serverConf["resource-path"].as<std::string>();
			app::host = serverConf["host"].as<std::string>();
			app::timeout = serverConf["timeout"].as<std::uint8_t>();
			logger->info("主服务初始化完成");
		}
		return 0;
	}() };
}
#endif // JUBEIMING_INIT_HPP  
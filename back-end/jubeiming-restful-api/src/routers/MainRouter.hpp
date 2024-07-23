/*
 * @File	  : MainRouter.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : 主路由定义
*/

#pragma once

#ifndef JUBEIMING_MAINROUTER_HPP
#define JUBEIMING_MAINROUTER_HPP

#include "define.hpp"
#include "IRouter.hpp"
#include "routers/AuthRouter.hpp"
#include "sw/redis++/redis++.h"
#include "common/mysql_conn.hpp"
#include "common/log.hpp"

namespace jubeiming::router {
	class MainRouter : public IRouter {
		std::shared_ptr<router::AuthRouter> mAuth;
		std::shared_ptr<sw::redis::Redis> mRedis;
		utils::MySQLConnector& mConn;
		std::shared_ptr<jubeiming::Log> mLogger;

	public:
		MainRouter(define::CrowApp, utils::MySQLConnector&, std::shared_ptr<sw::redis::Redis>, std::shared_ptr<jubeiming::Log>);
		virtual void setAuth(std::shared_ptr<router::AuthRouter>);
		virtual ~MainRouter() = default;
		virtual void startRouter();
	};
}

#endif // JUBEIMING_MAINROUTER_HPP
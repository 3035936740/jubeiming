/*
 * @File	  : IRouter.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : 路由接口
*/

#pragma once

#include "define.hpp"
#include "common/utils.hpp"

#ifndef JUBEIMING_IROUTER_HPP
#define JUBEIMING_IROUTER_HPP

#define App (*mApp)
#define DefRouter(ClassName) ClassName(define::CrowApp);
#define StartRouter(ClassName) ClassName::ClassName(define::CrowApp app) : IRouter(app){ \
using crow::HTTPMethod;

#define Router(Uri, Method, Content) CROW_ROUTE(App, Uri).methods(Method)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] Content );})

#define EndRouter }

namespace jubeiming::router {
	class IRouter {
	protected:
		define::CrowApp mApp;
	public:
		IRouter(define::CrowApp app) : mApp{ app } {};
	};
}

#endif // JUBEIMING_IROUTER_HPP
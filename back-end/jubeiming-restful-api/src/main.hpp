/*
 * @File	  : main.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2024/06/24 16:16
 * @Introduce : 主头文件
*/

#pragma once

#include <memory>
#include "define.hpp"
#include "boost/cobalt.hpp"
#include "common/exception.hpp"
#include "common/utils.hpp"
#include "boost/asio.hpp"
#include "init.hpp"

#ifndef JUBEIMING_MAIN_HPP
#define JUBEIMING_MAIN_HPP
using namespace boost;

#include "routers/IRouter.hpp"
#include "routers/AuthRouter.hpp"
#include "routers/MainRouter.hpp"

cobalt::task<int> run() {
    using namespace jubeiming::global;
    using namespace jubeiming::define;
    using namespace jubeiming;

    CrowApp app{ std::make_shared<CrowApplication>(CrowApplication()) };
    //关闭Crow的日志
    app->bindaddr(app::host).port(app::port);
    app->loglevel(crow::LogLevel::Info);
    app->timeout(app::timeout);

    // =====================================================

    auto auth{ std::make_shared<router::AuthRouter>(app, db::conns, gen::uuidInfo, db::redis, other::token, other::phoneVerify) };
    auth->setClient("WeiChatAPP", std::move(clients::WeiChatAPP));
    auth->setClient("Web", std::move(clients::Web));
    auth->setWeiChatAPPInfo(std::move(other::WeiChat));
    auth->setLogger(global::logger);
    auth->startRouter();
    router::MainRouter main(app, db::conns, db::redis, global::logger);
    main.setAuth(auth);
    main.startRouter();

    // =====================================================

    if (app::concurrency > 0) app->concurrency(app::concurrency).run_async();
    else if (app::concurrency == 0) app->multithreaded().run_async();
    else app->multithreaded().run();

	co_return 0;
}

#endif // JUBEIMING_MAIN_HPP  
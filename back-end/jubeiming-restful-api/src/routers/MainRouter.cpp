// 主路由实现

#include "MainRouter.hpp"
#include "services/sql.hpp"
#include "common/exception.hpp"
#include "nlohmann/json.hpp"
#include "fmt/core.h"
#include "crow.h"

namespace std {
    using fmt::format;
    using fmt::format_string;
    using fmt::formatter;
}

using crow::HTTPMethod;
using namespace jubeiming::router;

MainRouter::MainRouter(define::CrowApp app, utils::MySQLConnector& conn, std::shared_ptr<sw::redis::Redis> redis, std::shared_ptr<jubeiming::Log> logger) :
    IRouter(app),
    mConn{ conn },
    mRedis{ redis },
    mLogger{ logger }{

};

void jubeiming::router::MainRouter::setAuth(std::shared_ptr<router::AuthRouter> auth){
    mAuth = auth;
}

void jubeiming::router::MainRouter::startRouter(){
    CROW_ROUTE(App, "/me").methods(HTTPMethod::Get)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] {
        auto accessToken{ request.get_header_value("access_token") };

        if (accessToken.size() == 0) throw utils::HTTPException(401, nlohmann::json{
                {"status_code", define::code::TokenIsEmptyError},
                {"message", "access token is empty"}
                });
        auto userUUID{ mAuth->getAccessToken(accessToken).at("user_uuid").get<std::string>() };
        service::sql::ZhenNiu app(mConn);
        auto userInfos{ app.getUserByUUID(userUUID) };
        if(userInfos.size() == 0) {
            throw utils::HTTPException(404, nlohmann::json{
                    {"status_code", define::code::DataEmptyError},
                    {"message", "user infomation is empty"}
                });
        }
        const auto& userInfo{ userInfos.at(0) };
        
        nlohmann::json result{
            {"uuid", userUUID},
            {"user_status", userInfo.UserStatus.value_or(0)},
            {"user_level", userInfo.UserLevel.value_or(0)},
            {"user_grant", userInfo.UserGrant.value_or(0)}
        };

        if (userInfo.Scope.has_value()) result["scope"] = userInfo.Scope.value(); else result["scope"] = nullptr;
        if (userInfo.UserName.has_value()) result["username"] = userInfo.UserName.value(); else result["username"] = nullptr;
        if (userInfo.Email.has_value()) result["email"] = userInfo.Email.value(); else result["email"] = nullptr;
        if (userInfo.PhoneNumber.has_value()) result["phone_number"] = userInfo.PhoneNumber.value(); else result["phone_number"] = nullptr;
        if (userInfo.RegistrationDate.has_value()) result["registration_date"] = userInfo.RegistrationDate.value().as_time_point().time_since_epoch().count();
        else result["registration_date"] = nullptr;

        return result.dump();
        });
        });

    CROW_ROUTE(App, "/me/web").methods(HTTPMethod::Get)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] {
        auto accessToken{ request.get_header_value("access_token") };

        if (accessToken.size() == 0) throw utils::HTTPException(401, nlohmann::json{
                {"status_code", define::code::TokenIsEmptyError},
                {"message", "access token is empty"}
                });
        auto userUUID{ mAuth->getAccessToken(accessToken).at("user_uuid").get<std::string>() };
        service::sql::WebApp app(mConn);
        auto webInfos{ app.getWebAppByUUID(userUUID) };
        if(webInfos.size() == 0) {
            throw utils::HTTPException(404, nlohmann::json{
                    {"status_code", define::code::DataEmptyError},
                    {"message", "user infomation is empty"}
                });
        }
        const auto& webInfo{ webInfos.at(0) };

        nlohmann::json result{
            {"id", webInfo.ID},
            {"uuid", webInfo.UserUUID},
            {"avatar_url", webInfo.AvatarUrl.value_or("")},
        };
        return result.dump();
        });
        });

    CROW_ROUTE(App, "/wei/me").methods(HTTPMethod::Get)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] {
        auto accessToken{ request.get_header_value("access_token") };

        if (accessToken.size() == 0) throw utils::HTTPException(401, nlohmann::json{
                {"status_code", define::code::TokenIsEmptyError},
                {"message", "access token is empty"}
                });
        auto userUUID{ mAuth->getAccessToken(accessToken).at("user_uuid").get<std::string>() };
        service::sql::ZhenNiu app(mConn);
        auto userInfos{ app.getUserByUUID(userUUID) };
        if(userInfos.size() == 0) {
            throw utils::HTTPException(404, nlohmann::json{
                    {"status_code", define::code::DataEmptyError},
                    {"message", "user infomation is empty"}
                });
        }
        const auto& userInfo{ userInfos.at(0) };
        
        nlohmann::json result{
            {"uuid", userUUID},
            {"user_status", userInfo.UserStatus.value_or(0)},
            {"user_level", userInfo.UserLevel.value_or(0)},
            {"user_grant", userInfo.UserGrant.value_or(0)}
        };

        if (userInfo.Scope.has_value()) result["scope"] = userInfo.Scope.value(); else result["scope"] = nullptr;
        if (userInfo.UserName.has_value()) result["username"] = userInfo.UserName.value(); else result["username"] = nullptr;
        if (userInfo.Email.has_value()) result["email"] = userInfo.Email.value(); else result["email"] = nullptr;
        if (userInfo.PhoneNumber.has_value()) result["phone_number"] = userInfo.PhoneNumber.value(); else result["phone_number"] = nullptr;
        if (userInfo.RegistrationDate.has_value()) result["registration_date"] = userInfo.RegistrationDate.value().as_time_point().time_since_epoch().count();
        else result["registration_date"] = nullptr;

        return result.dump();
        });
        });

    // TODO 修改手机号 / 没有短信服务
    CROW_ROUTE(App, "/www").methods(HTTPMethod::Get)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] {
        return "ok";

        });
        });
    
    //PublishArticle
    CROW_ROUTE(App, "/publish_article").methods(HTTPMethod::Post)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] {
        return "ok";
        });
        });

    // TODO 没有开通付费相关无法进行付费升级
    CROW_ROUTE(App, "/upgrade/level").methods(HTTPMethod::Get)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] {

        // ======================
        return "ok";
        // ======================

        auto accessToken{ request.get_header_value("access_token") };

        if (accessToken.size() == 0) throw utils::HTTPException(401, nlohmann::json{
                {"status_code", define::code::TokenIsEmptyError},
                {"message", "access token is empty"}
            });
        auto userUUID{ mAuth->getAccessToken(accessToken).at("user_uuid").get<std::string>() };

        service::sql::ZhenNiu app(mConn);

        app.modifyUserLevel("UserLevel = UserLevel + 1", userUUID);

        return "ok";
        });
        });
}

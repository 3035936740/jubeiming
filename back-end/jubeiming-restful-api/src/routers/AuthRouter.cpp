// oauth2的实现
#include "AuthRouter.hpp"
#include "common/exception.hpp"
#include "common/phone_verify.hpp"
#include "fmt/core.h"
#include "crow.h"
#include "services/sql.hpp"
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include <curlpp/Infos.hpp>
#include "curlpp/Options.hpp"
#include <sstream>
#include <future>
#include "fmt/core.h"

namespace KEY {
	constexpr const char* REFRESH_TOKEN{ "refresh_token" };
	constexpr const char* ACCESS_TOKEN{ "access_token" };
}

std::string genToken() {
	return jubeiming::utils::generateRandom(16UL, "0123456789abcdefghijklmnopqrstuvwxyz");
}

namespace std {
	using fmt::format;
	using fmt::format_string;
	using fmt::formatter;
}

using namespace jubeiming::router;
using nlohmann::json;

nlohmann::json jubeiming::router::AuthRouter::getRefreshToken(std::string_view refreshToken) {
	std::vector<std::string> keys;

	std::string refreshTokenKey{ fmt::format("{}:{}:*", KEY::REFRESH_TOKEN, refreshToken) };

	mRedis->keys(refreshTokenKey, std::back_inserter(keys));
	if (keys.size() == 0) {
		throw utils::HTTPException(401, json{
							{"status_code", define::code::TokenExpireError},
							{"message", "refresh token expires or doesn't exist"}
			});
	}

	refreshTokenKey.clear();
	refreshTokenKey = keys.at(0);

	auto refreshTokenOptional{ mRedis->get(refreshTokenKey) };
	if (not refreshTokenOptional.has_value()) throw utils::HTTPException(401, json{
							{"status_code", define::code::TokenNotExistError},
							{"message", "refresh token expires"}
		});

	json refreshTokenJson{ json::parse(refreshTokenOptional.value()) };
	std::exchange(refreshTokenJson, refreshTokenJson[0]);
	return refreshTokenJson;
}

nlohmann::json jubeiming::router::AuthRouter::getAccessToken(std::string_view accessToken) {
	std::vector<std::string> keys;

	std::string accessTokenKey{ fmt::format("{}:{}:*", KEY::ACCESS_TOKEN, accessToken) };

	mRedis->keys(accessTokenKey, std::back_inserter(keys));
	if (keys.size() == 0) {
		throw utils::HTTPException(401, json{
							{"status_code", define::code::TokenExpireError},
							{"message", "access token expires or doesn't exist"}
			});
	}

	accessTokenKey.clear();
	accessTokenKey = keys.at(0);

	auto accessTokenOptional{ mRedis->get(accessTokenKey) };
	if (not accessTokenOptional.has_value()) throw utils::HTTPException(401, json{
							{"status_code", define::code::TokenNotExistError},
							{"message", "access token expires"}
		});

	json accessTokenJson{ json::parse(accessTokenOptional.value()) };
	std::exchange(accessTokenJson, accessTokenJson[0]);
	return accessTokenJson;
}

void jubeiming::router::AuthRouter::VerifyCodeMethod(const std::string& phoneNumber, const std::string& verificationCode, const std::string& remote_ip, const std::string& region_code){
	if (region_code != "+86") {
		throw utils::HTTPException(400, json{
							{"status_code", define::code::NotSupport},
							{"message", region_code + " not support"}
			});
	}

	if (phoneNumber.empty()) {
		throw utils::HTTPException(400, json{
							{"status_code", define::code::DataEmptyError},
							{"message", "phone_number is empty"}
			});
	}

	std::vector<std::string> keys;

	mRedis->keys(fmt::format("verify_code:*:{}", phoneNumber), std::back_inserter(keys));
	if (keys.size() == 0) {
		throw utils::HTTPException(401, json{
							{"status_code", define::code::VerifyNotExistError},
							{"message", "verification code doesn't exist"}
			});
	}

	keys.clear();

	std::string phoneVerifyKey{ fmt::format("verify_code:{}:{}", remote_ip, phoneNumber) };

	auto verifyCodeOpt{ mRedis->get(phoneVerifyKey) };

	if (not verifyCodeOpt.has_value()) throw utils::HTTPException(401, json{
					{"status_code", define::code::VerifyNotExistError},
					{"message", "verification code doesn't exist"}
		});

	auto verifyCode{ verifyCodeOpt.value() };

	if (verifyCode != verificationCode) throw utils::HTTPException(401, json{
					{"status_code", define::code::VerifyCodeIncorrect},
					{"message", "incorrect verification code"}
		});

	mRedis->del(phoneVerifyKey);
}

std::string jubeiming::router::AuthRouter::createRefreshToken(const std::string& uuid) {
	std::vector<std::string> keys;

	mRedis->keys(fmt::format("{}:*:{}", KEY::REFRESH_TOKEN, uuid), std::back_inserter(keys));
	if (keys.size() > 0) {
		for (auto& key : keys) {
			mRedis->del(key);
		}
	}

	jubeiming::service::sql::ZhenNiu app(mConn);
	auto userInfo{ app.getUserByUUID(uuid).at(0) };

	std::string refreshToken;
	while (true) {
		std::string refreshTokenTemp{ genToken() };

		keys.clear();
		mRedis->keys(fmt::format("{}:{}:*", KEY::REFRESH_TOKEN, refreshTokenTemp), std::back_inserter(keys));
		if (keys.size() == 0) {
			refreshToken = std::move(refreshTokenTemp);
			break;
		}
	}

	auto key{ fmt::format("{}:{}:{}", KEY::REFRESH_TOKEN, refreshToken, uuid) };

	json refreshData{
		{"user_uuid", userInfo.UserUUID},
	};

	mRedis->set(key, refreshData.dump());
	mRedis->expire(key, mTokenInfo.refreshExpire);

	return key;
}

std::string jubeiming::router::AuthRouter::createAccessToken(const std::string& uuid) {
	std::vector<std::string> keys;

	mRedis->keys(fmt::format("{}:*:{}", KEY::ACCESS_TOKEN, uuid), std::back_inserter(keys));
	if (keys.size() > 0) {
		for (auto& key : keys) {
			mRedis->del(key);
		}
	}

	jubeiming::service::sql::ZhenNiu app(mConn);
	auto userInfo{ app.getUserByUUID(uuid).at(0) };

	std::string accessToken;

	while (true) {
		std::string accessTokenTemp{ genToken() };

		keys.clear();
		mRedis->keys(fmt::format("{}:{}:*", KEY::ACCESS_TOKEN, accessTokenTemp), std::back_inserter(keys));
		if (keys.size() == 0) {
			accessToken = std::move(accessTokenTemp);
			break;
		}
	}

	auto key{ fmt::format("{}:{}:{}", KEY::ACCESS_TOKEN, accessToken, uuid) };

	json accessData{
		{"user_uuid", userInfo.UserUUID},
	};

	if (userInfo.RegistrationDate.has_value()) accessData["registration_date"] = userInfo.RegistrationDate.value().as_time_point().time_since_epoch().count();
	else accessData["registration_date"] = nullptr;


	mRedis->set(key, accessData.dump());
	mRedis->expire(key, mTokenInfo.accessExpire);

	return key;
}

std::string jubeiming::router::AuthRouter::getAccessTokenWarpper(const std::string& key) {
	json refreshTokenJson{ getRefreshToken(key) };
	std::exchange(refreshTokenJson, refreshTokenJson[0]);

	const std::string userUUID{ refreshTokenJson.at("user_uuid").get<std::string>() };
	auto accessToken{ createAccessToken(userUUID) };
	return accessToken;
}

AuthRouter::AuthRouter(define::CrowApp app, utils::MySQLConnector& conn, const jubeiming::define::UuidInfo& uuidinfo, std::shared_ptr<sw::redis::Redis> redis, const jubeiming::define::TokenInfo& tokenInfo, const jubeiming::define::PhoneVerify& phoneVerify) :
	IRouter(app),
	mConn{ conn },
	mUuidInfo{ uuidinfo },
	mRedis{ redis },
	mTokenInfo{ tokenInfo },
	mPhoneVerify{ phoneVerify } {}

void jubeiming::router::AuthRouter::setClient(std::string_view name, define::sql::client&& client) {
	mClient[name.data()] = client;
}

void jubeiming::router::AuthRouter::setWeiChatAPPInfo(define::WeiChatApp&& info)
{
	mWeiChatAppInfo = info;
}

void jubeiming::router::AuthRouter::setLogger(std::shared_ptr<jubeiming::Log> log) {
	mLogger = log;
}

void jubeiming::router::AuthRouter::startRouter()
{
	using crow::HTTPMethod;
	CROW_ROUTE(App, "/auth/token").methods(HTTPMethod::Post)([&](const crow::request& request) { return jubeiming::utils::HandleResponseBody([&] {
		auto remote_ip{ request.remote_ip_address };
		json result;
		std::stringstream ss;
		std::string uniqueId;
		ss << std::hex << &request << std::dec;
		uniqueId = ss.str();
		ss.str("");
		json data{ json::parse(request.body) };
		std::exchange(data, data[0]);

		std::string grantType{ data.at("grant_type").get<std::string>() };

		if (grantType == "codeweichat") {
			ss << "ID:" << uniqueId << " => 微信小程序流程";
			mLogger->info(ss.str());
			ss.str("");
			// 微信小程序授权取得的openid
			service::sql::Auth auth(mConn);

			const auto& weiChatAppClient{ mClient.at("WeiChatAPP") };

			auto clientRecords{ auth.getClientByUUID(weiChatAppClient.ClientUUID) };

			if (clientRecords.empty()) {
				constexpr const char* describe{ "uuid doesn't exist" };
				mLogger->error("ID:" + uniqueId + " => " + describe);
				throw utils::HTTPException(500, describe);
			}
			auto clientRecord{ clientRecords.at(0) };
			if (weiChatAppClient.ClientSecret != clientRecord.ClientSecret) {
				constexpr const char* describe{ "secret error" };
				mLogger->error("ID:" + uniqueId + " => " + describe);
				throw utils::HTTPException(500, describe);
			}

			std::string loginCode{ data.at("login_code").get<std::string>() };
			std::string phoneNumberCode{ data.at("phone_number_code").get<std::string>() };
			ss << "ID:" << uniqueId << " / Login Code: " << loginCode << " / Phone Number Code: " << phoneNumberCode;
			mLogger->info(ss.str());
			ss.str("");

			json weiXinResponse;

			json weiXinPhoneNumber;

			// 登入获取
			std::future<json> getLoginRecord{ std::async(std::launch::async, [&] {
				std::list<std::string> header;
				std::ostringstream stream;
				header.push_back("Content-Type: application/json");
				curlpp::Easy request;
				request.setOpt<curlpp::options::HttpHeader>(header);
				// Set the URL.
				request.setOpt<curlpp::options::Url>(mWeiChatAppInfo.url + "/sns/jscode2session?" + utils::ParamsString({
					{"appid", mWeiChatAppInfo.appid},
					{"secret", mWeiChatAppInfo.secret},
					{"grant_type", "authorization_code"},
					{"js_code", loginCode}
					}));
				request.setOpt<curlpp::options::SslEngineDefault>({});
				request.setOpt<curlpp::options::HttpGet>({});
				request.setOpt<curlpp::options::WriteStream>(&stream);
				request.perform();
				auto httpCode{ curlpp::infos::ResponseCode::get(request) };

				if (httpCode >= 400 or httpCode < 200) {
					constexpr const char* describe{ "weixin code error" };
					mLogger->error("ID:" + uniqueId + " => " + describe);
					throw utils::HTTPException(403, describe);
				}
				auto response = json::parse(stream.str());

				if (not response.count("openid")) {
					mLogger->error("ID:" + uniqueId + " => code非法");
					throw utils::HTTPException(401, response);
				}
				return response;
				// 祝贺通过授权!!
				}) };
			// 手机号获取逻辑
			std::future<json> getPhoneNumberRecord{ std::async(std::launch::deferred,[&] {
				std::list<std::string> header;
				std::ostringstream stream;
				header.push_back("Content-Type: application/json");
				curlpp::Easy request;
				request.setOpt<curlpp::options::HttpHeader>(header);
				// Set the URL.
				request.setOpt<curlpp::options::Url>(mWeiChatAppInfo.url + "/cgi-bin/token?" + utils::ParamsString({
					{"appid", mWeiChatAppInfo.appid},
					{"secret", mWeiChatAppInfo.secret},
					{"grant_type", "client_credential"},
					}));
				request.setOpt<curlpp::options::SslEngineDefault>({});
				request.setOpt<curlpp::options::HttpGet>({});
				request.setOpt<curlpp::options::WriteStream>(&stream);
				request.perform();
				auto httpCode{ curlpp::infos::ResponseCode::get(request) };
				if (httpCode >= 400 or httpCode < 200) {
					constexpr const char* describe{ "weixin code error" };
					mLogger->error("ID:" + uniqueId + " => " + describe);
					throw utils::HTTPException(403, describe);
				}
				auto response{ json::parse(stream.str()) };
				std::exchange(response, response[0]);
				stream.str("");

				if (not response.count("access_token")) {
					mLogger->error("ID:" + uniqueId + " => access token非法");
					throw utils::HTTPException(401, response);
				}
				std::string accessToken{ response["access_token"].get<std::string>() };

				request.reset();
				request.setOpt<curlpp::options::HttpHeader>(header);

				request.setOpt<curlpp::options::Url>(mWeiChatAppInfo.url + "/wxa/business/getuserphonenumber?" + utils::ParamsString({
					{"access_token", accessToken}
					}));
				request.setOpt<curlpp::options::SslEngineDefault>({});
				request.setOpt<curlpp::options::PostFields>("{\"code\": \"" + phoneNumberCode + "\"}");
				request.setOpt<curlpp::options::WriteStream>(&stream);
				request.perform();
				httpCode = curlpp::infos::ResponseCode::get(request);
				if (httpCode >= 400 or httpCode < 200) {
					constexpr const char* describe{ "weixin code error" };
					mLogger->error("ID:" + uniqueId + " => " + describe);
					throw utils::HTTPException(403, describe);
				}
				response = json::parse(stream.str());

				if (not response.count("phone_info")) {
					mLogger->error("ID:" + uniqueId + " => 无法获取手机号(phone_info)相关信息");
					throw utils::HTTPException(401, response);
				}

				return response;
			}) };

			weiXinResponse = getLoginRecord.get();
			weiXinPhoneNumber = getPhoneNumberRecord.get();

			mLogger->info("ID:" + uniqueId + " => 微信授权通过");

			auto userUuid{ utils::UUIDGenerator(mUuidInfo) };

			std::exchange(weiXinPhoneNumber, weiXinPhoneNumber["phone_info"]);

			std::string phoneNumber{ "+" + weiXinPhoneNumber["countryCode"].get<std::string>() + " " + weiXinPhoneNumber["purePhoneNumber"].get<std::string>() };

			auto openid{ weiXinResponse.at("openid").get<std::string>() };

			service::sql::ZhenNiu app(mConn);

			auto userInfo{ app.getUserByPhoneNumber(phoneNumber) };
			auto weiAppInfo{ app.getWeiAppInfoByOpenID(openid) };

			if (not weiAppInfo.has_value()) {
				app.addNewWeiChatAPPAccount(define::sql::WeiApp{
					.UserUUID = userUuid,
					.OpenID = openid
					},
					define::sql::user{
					.UserUUID = userUuid,
					.PhoneNumber = phoneNumber
					});
				auto newUserInfo{ app.getUserByPhoneNumber(phoneNumber) };

				std::exchange(userInfo, newUserInfo);

				auto newWeiAppInfo{ app.getWeiAppInfoByOpenID(openid) };

				std::exchange(weiAppInfo, newWeiAppInfo);
				mLogger->info("ID:" + uniqueId + " => 账户添加完成");
			}
			else {
				if (not userInfo.has_value()) {
					mLogger->info("ID:" + uniqueId + " => 手机号码并不一致");
					throw utils::HTTPException(401, json{
						{"status_code", define::code::PhoneInconsistentError},
						{"message", "Inconsistent phone numbers"}
						});
				}
				mLogger->info("ID:" + uniqueId + " => 这个账户已经绑定正在试图登入");
			}

			auto refreshToken{ utils::strSplit(createRefreshToken(userInfo.value().UserUUID), ":").at(1) };
			auto accessToken{ utils::strSplit(getAccessTokenWarpper(refreshToken), ":").at(1) };
			result["access_token"] = accessToken;
			result["refresh_token"] = refreshToken;

			std::time_t now_time_t{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };

			result["expires_in"] = now_time_t + mTokenInfo.accessExpire;
			result["token_type"] = "Bearer";

			ss.str("");
			ss << "ID:" << uniqueId << "小程序授权完成";
			mLogger->info(ss.str());
		}
		else if (grantType == "reg_login")
		{
			ss << "ID:" << uniqueId << " => 普通登入流程";
			mLogger->info(ss.str());
			ss.str("");
			// 微信小程序授权取得的openid
			service::sql::Auth auth(mConn);

			const auto& webClient{ mClient.at("Web") };

			auto clientRecords{ auth.getClientByUUID(webClient.ClientUUID) };

			if (clientRecords.empty()) {
				constexpr const char* describe{ "uuid doesn't exist" };
				mLogger->error("ID:" + uniqueId + " => " + describe);
				throw utils::HTTPException(500, describe);
			}

			auto clientRecord{ clientRecords.at(0) };

			if (webClient.ClientSecret != clientRecord.ClientSecret) {
				constexpr const char* describe{ "secret error" };
				mLogger->error("ID:" + uniqueId + " => " + describe);
				throw utils::HTTPException(500, describe);
			}

			std::string phoneNumber{ data.at("phone_number").get<std::string>() };
			std::string verificationCode{ data.at("verification_code").get<std::string>() };
			std::string regionCode{ "+86" };

			if (data.count("region_code") > 0) {
				regionCode = data.at("region_code").get<std::string>();
			}

			mLogger->info(fmt::format("ID: {} => Phone Number: {}", uniqueId, phoneNumber));
			mLogger->info(fmt::format("ID: {} => Verification Code: {}", uniqueId, verificationCode));
			mLogger->info(fmt::format("ID: {} => Region Code: {}", uniqueId, regionCode));

			VerifyCodeMethod(phoneNumber, verificationCode, remote_ip, regionCode);

			service::sql::ZhenNiu app(mConn);

			auto userInfoOpt{ app.getUserByPhoneNumber(regionCode + " " + phoneNumber)};

			auto userUuid{ utils::UUIDGenerator(mUuidInfo) };

			if (userInfoOpt.has_value())
			{
				auto userInfo{ userInfoOpt.value() };
				mLogger->info("ID:" + uniqueId + " => 账户已存在 -> " + userInfo.UserUUID);

				auto refreshToken{ utils::strSplit(createRefreshToken(userInfo.UserUUID), ":").at(1) };
				auto accessToken{ utils::strSplit(getAccessTokenWarpper(refreshToken), ":").at(1) };
				result["access_token"] = accessToken;
				result["refresh_token"] = refreshToken;

				std::time_t now_time_t{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };

				result["expires_in"] = now_time_t + mTokenInfo.accessExpire;
				result["token_type"] = "Bearer";
			}
			else
			{
				app.addNewAccount({
					.UserUUID = userUuid,
					.PhoneNumber = regionCode + " " + phoneNumber
					});

				auto userInfos{ app.getUserByUUID(userUuid) };

				auto userInfo{ userInfos.at(0) };

				auto refreshToken{ utils::strSplit(createRefreshToken(userInfo.UserUUID), ":").at(1) };
				auto accessToken{ utils::strSplit(getAccessTokenWarpper(refreshToken), ":").at(1) };
				result["access_token"] = accessToken;
				result["refresh_token"] = refreshToken;

				std::time_t now_time_t{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };

				result["expires_in"] = now_time_t + mTokenInfo.accessExpire;
				result["token_type"] = "Bearer";
				mLogger->info("ID:" + uniqueId + " => 账户添加完成");
			}

		}
		else if (grantType == "refresh_token")
		{
			ss << "ID:" << uniqueId << " => 刷新access token";
			mLogger->info(ss.str());
			auto refreshToken{ data.at("refresh_token").get<std::string>() };
			auto accessToken{ utils::strSplit(getAccessTokenWarpper(refreshToken), ":").at(1) };

			ss.str("");
			ss << "ID:" << uniqueId << " => refresh_token: " << refreshToken;
			mLogger->info(ss.str());

			result["access_token"] = accessToken;

			std::time_t now_time_t{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };

			result["expires_in"] = now_time_t + mTokenInfo.accessExpire;
			result["token_type"] = "Bearer";

			ss.str("");
			ss << "ID:" << uniqueId << "刷新access token完成";
			mLogger->info(ss.str());
		}
		else
		{
			throw utils::HTTPException(401, "unsupported grant type"s);
		}

		return result.dump(2);
		});
		});

	CROW_ROUTE(App, "/verify/send").methods(HTTPMethod::Post)([&](const crow::request& request) {
		auto remote_ip{ request.remote_ip_address };
		json data{ json::parse(request.body) };

		std::stringstream ss;
		ss << std::hex << &request << std::dec;
		auto uniqueId{ ss.str() };
		ss.str("");

		std::exchange(data, data[0]);
		bool exist_record{ false };
		return jubeiming::utils::HandleResponseBody([&] {
			json result;

			std::vector<std::string> keys;
			std::string phoneNumber{ data.at("phone_number").get<std::string>() };

			std::string phoneVerifyKey{ fmt::format("verify_code:{}:{}", remote_ip, phoneNumber) };

			mLogger->info(fmt::format("ID: {} => Phone Number: {}", uniqueId, phoneNumber));
			mLogger->info(fmt::format("ID: {} => Key: {}", uniqueId, phoneVerifyKey));

			if (phoneNumber.empty()) {
				throw utils::HTTPException(400, json{
									{"status_code", define::code::DataEmptyError},
									{"message", "phone_number is empty"}
					});
			}

			std::string verifyCode{ utils::generateRandom(6, "0123456789") };
			mLogger->info(fmt::format("ID: {} => Verify Code: {}", uniqueId, verifyCode));

			// IP verify
			mRedis->keys(fmt::format("verify_code:{}:*", remote_ip), std::back_inserter(keys));
			if (keys.size() != 0) {
				exist_record = true;
			}
			keys.clear();
			mRedis->keys(fmt::format("verify_code:*:{}", phoneNumber), std::back_inserter(keys));
			if (keys.size() != 0) {
				exist_record = true;
			}
			keys.clear();

			if (exist_record) {
				auto ttl{ mRedis->ttl(phoneVerifyKey) };
				result["ttl"] = ttl;
				result["status"] = define::code::Exist;

				mLogger->warn(fmt::format("ID: {} => Has Exist -> TTL: {}", uniqueId, ttl));

				return result.dump();
			}

			// refreshTokenKey = keys.at(0);
			mRedis->set(phoneVerifyKey, verifyCode);
			mRedis->expire(phoneVerifyKey, mPhoneVerify.codeValidity);

			utils::sendPhoneVerify(mPhoneVerify, phoneNumber, verifyCode);

			result["ttl"] = mPhoneVerify.codeValidity;
			result["status"] = define::code::Success;
			mLogger->info(fmt::format("ID: {} => Complete -> TTL: {}", uniqueId, mPhoneVerify.codeValidity));
			return result.dump();
			});
		});

	CROW_ROUTE(App, "/verify/verify_code").methods(HTTPMethod::Post)([&](const crow::request& request) {
		auto remote_ip{ request.remote_ip_address };
		json data{ json::parse(request.body) };

		std::stringstream ss;
		ss << std::hex << &request << std::dec;
		auto uniqueId{ ss.str() };
		ss.str("");

		std::exchange(data, data[0]);
		return jubeiming::utils::HandleResponseBody([&] {
			std::string phoneNumber{ data.at("phone_number").get<std::string>() };
			std::string verificationCode{ data.at("verification_code").get<std::string>() };
			std::string regionCode{ "+86" };

			if (data.count("region_code") > 0) {
				regionCode = data.at("region_code").get<std::string>();
			}

			mLogger->info("ID:" + uniqueId + " => 进行效验");
			mLogger->info("ID:" + uniqueId + " => " + regionCode + " " + phoneNumber + " / " + verificationCode);

			VerifyCodeMethod(phoneNumber, verificationCode, remote_ip, regionCode);

			mLogger->info("ID:" + uniqueId + " => 效验成功");
			return "ok";
			});
		});
};

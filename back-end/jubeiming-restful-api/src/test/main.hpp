/*
 * @File	  : sql.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : SQL定义
*/

#pragma once

#ifndef JUBEIMING_TEST_HPP
#define JUBEIMING_TEST_HPP
#include <iostream>
#include "services/sql.hpp"
#include "bcrypt.h"
#include "init.hpp"
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include <curlpp/Infos.hpp>
#include "curlpp/Options.hpp"
#include "common/utils.hpp"
#include "common/WXBizDataCrypt.h"
#include "fmt/core.h"
#include "common/phone_verify.hpp"

void test_db() {
	using namespace jubeiming::service;
	using namespace jubeiming;
	using jubeiming::global::logger;
	std::string password{ "www.jubeiming.com" };
	std::string hash{ bcrypt::generateHash(password) };
	logger->info(password);
	logger->info(hash);
	std::stringstream ss;
	ss << std::boolalpha << bcrypt::validatePassword(password, hash);
	logger->info("is equal => "s + ss.str());
	sql::ZhenNiu user(global::db::conns);

	auto userinfos{ user.getUserByID(2) };

	if (not userinfos.empty())
	{
		logger->info(std::to_string(userinfos[0].RegistrationDate.value().as_time_point().time_since_epoch().count()));
	}
	else {
		logger->info("Empty");
	}

	auto value{ global::db::redis->get("testkey") };
	if (value)logger->info(fmt::format("redis value is :{}", *value));
	else logger->info("key doesn't exist");
};

void test_curl() {
	using namespace curlpp::options;

	try
	{
		// Our request to be sent.
		curlpp::Easy request;

		std::ostringstream stream;

		std::list<std::string> header;
		header.push_back("Content-Type: application/json");
		// Set the URL.
		request.setOpt<curlpp::options::Url>("https://yesno.wtf/api"s);
		request.setOpt<curlpp::options::SslEngineDefault>({});
		request.setOpt<curlpp::options::HttpGet>({});
		request.setOpt<curlpp::options::HttpHeader>(header);
		// request.setOpt<curlpp::options::Verbose>(true);
		request.setOpt<curlpp::options::WriteStream>(&stream);

		// Send request and get a result.
		// By default the result goes to standard output.
		request.perform();
		
		auto http_code{ curlpp::infos::ResponseCode::get(request) };
		
		std::ostringstream response_stream;
		response_stream << request;
		std::string response = response_stream.str();

		std::cout << "/bad got code: " << http_code << " And content: " << stream.str() << std::endl;
		std::cout << response << std::endl;
	}
	catch (curlpp::RuntimeError& e)
	{
		std::cout << e.what() << std::endl;
	}

	catch (curlpp::LogicError& e)
	{
		std::cout << e.what() << std::endl;
	}
};

void test_utils() {
	auto  param{ jubeiming::utils::ParamsString({
		{"test", "wtass"},
		{"test2", "w234tass"},
		{"43test2", "w2hgswaz34tass"},
		}) };
	std::cout << param << std::endl;
};

void test_weiunionid() {
	using namespace std;
	cout << "start to test" << endl << endl << endl;
	string sSessionkey = "BjihLl1I5yhO2j9CMEafyA==";
	string sAppid = "wx94ad0e4960761a41";
	jubeiming::utils::WxBizDataSecure::WXBizDataCrypt oWXBizDataCrypt(sAppid, sSessionkey);

	//decrypt
	string sEncryptedData = "XCjNm/KeryvszJ7GKesOFWahJQsXDPmPb7Oqhhx6K/+EwiK1BETtuWFH957x70JJ/R37JIiErPir+MibclUcFQZqL1lWKDeBhgaJ7kpB2O2UnNVJlHMln10ZLcvAIq/guxujqU7js0T8dFQ4oZXQuv71MoJo/g3aAm2BWhUKlbL1ul1/LpSimiwv12VW3dtUUEEAxwKnz5gQypXmBm0dfcwyVgML1aE3H64sqk9Rm7qhHAQ+ssBpxrZxkilrUk0MfFdOiA6UAm6QeVADCpuW/zHfPx3jIj0XOTFBJv4S+soF8XBs33op9QU3tHk1kT+0YTH02oD0iND7zzDy6KTCg5/7IYyBdRAJ/KI1EWA+Ya1gv7SRoUqawRgDrUWUSFtanmBfCuhYqkayvlWL7gr8pLEN1gLcqt63eU6QWh7/9aHtqgTeRasKVMjBBScnqhhO1D5WaEMhlRzdA/G8Q9F8PA==";

	string sIv = "DgLMDrewfNjQL4HIG6bE9Q==";


	cout << "sEncryptBase64 size:" << sEncryptedData.size() << endl;
	cout << "sIv size:" << sIv.size() << endl;

	string sData;

	int ret = oWXBizDataCrypt.DecryptData(sEncryptedData, sIv, sData);
	cout << "DecryptData ret:" << ret << " size:" << sData.size() << " data:" << endl;
	cout << sData << endl << endl << endl;
}

void test_phone_verify() {
	auto [code, resp] { jubeiming::utils::sendPhoneVerify(jubeiming::global::other::phoneVerify, "19883988084", "123456")};
	std::cout << "code:\n" << code << "\nresp:\n" << resp << std::endl;
};

void test_redis() {
	using namespace jubeiming::global;
	std::cout << db::redis->ttl("refresh_token:ayb0fn55ued1ddhv:66405fd9-f3d0-55dc-b445-ab5a423fbe60") << std::endl;
}

int starta_test() {
	//test_db();
	//test_curl();
	// test_utils();
	// test_weiunionid();
	// test_phone_verify();
	test_redis();
	return EXIT_SUCCESS;
}

#endif // JUBEIMING_TEST_HPP
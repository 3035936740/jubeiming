/*
 * @File	  : phone_verify.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : 手机效验工具
*/

#pragma once

#ifndef JUBEIMING_PHONE_VERIFY_HPP
#define JUBEIMING_PHONE_VERIFY_HPP

#include "define.hpp"
#include "common/exception.hpp"
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Infos.hpp"
#include "curlpp/Options.hpp"
#include "nlohmann/json.hpp"

namespace jubeiming::utils {
	auto sendPhoneVerify(const jubeiming::define::PhoneVerify& phone_verify, const std::string& phoneNumber, const std::string& verificationCode) {
		std::list<std::string> header;
		header.push_back("Content-Type: application/json");
		header.push_back("Authorization: " + phone_verify.authorization);
		std::ostringstream stream;
		curlpp::Easy request;
		request.setOpt<curlpp::options::Url>(phone_verify.url + "/send");
		request.setOpt<curlpp::options::SslEngineDefault>({});
		request.setOpt<curlpp::options::PostFields>(nlohmann::json{
			{"phone_number", phoneNumber},
			{"verification_code", verificationCode}
			}.dump());
		request.setOpt<curlpp::options::HttpHeader>(header);
		request.setOpt<curlpp::options::WriteStream>(&stream);
		// request.setOpt<curlpp::options::Verbose>(true);
		request.perform();
		auto httpCode{ curlpp::infos::ResponseCode::get(request) };
		auto response{ nlohmann::json::parse(stream.str()) };
		if (httpCode >= 400 or httpCode < 200) {
			throw utils::HTTPException(httpCode, response);
		}

		return std::tuple{ httpCode, response };
	}
}

#endif // JUBEIMING_PHONE_VERIFY_HPP
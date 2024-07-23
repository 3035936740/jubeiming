// 工具实现
#include "utils.hpp"
#include <random>
#include <ranges>
#include <string>
#include <vector>
#include <functional>
#include "exception.hpp"
#include "http_util.hpp"
#include "Poco/SHA1Engine.h"
#include "Poco/UUID.h"
#include "Poco/DigestEngine.h"
#include "Poco/UUIDGenerator.h"

using nlohmann::json;

void jubeiming::utils::Timer::start() {
	start_time = std::chrono::high_resolution_clock::now();
}

void jubeiming::utils::Timer::stop() {
	end_time = std::chrono::high_resolution_clock::now();
}

double jubeiming::utils::Timer::elapsedMilliseconds() const {
	return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
}

double jubeiming::utils::Timer::elapsedSeconds() const {
	return std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
}

std::string jubeiming::utils::strRemove(const std::string& fullString, const std::string& substring) {
	std::size_t found = fullString.find(substring);
	if (found != std::string::npos) {
		return fullString.substr(found + substring.length());
	}
	else {
		throw std::runtime_error("Substring not found in the string.");
	}
}

std::vector<std::string> jubeiming::utils::strSplit(const std::string& str, std::string_view delimiter) {
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find(delimiter);

	while (end != std::string::npos) {
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}

	tokens.push_back(str.substr(start)); // 添加最后一个片段
	return tokens;
}

std::string jubeiming::utils::generateRandom(size_t length, const std::string& charset) {
	std::random_device seeder; // 用于生成随机种子    mt19937 gen(rd()); // 以随机种子初始化随机数生成器

	const auto seed{ seeder.entropy() ? seeder() : time(nullptr) };

	std::mt19937 engine{ static_cast<std::mt19937::result_type>(seed) };

	std::uniform_int_distribution<size_t> distribution{ 0, charset.length() - 1 }; // 均匀分布

	std::string randomStr;
	for (size_t i = 0; i < length; i++) {
		randomStr += charset[distribution(engine)]; // 从字符集中随机选择一个字符    
	}
	return randomStr;
}

std::string jubeiming::utils::retainDecimalPlaces(double f, int n) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(n) << f;
	return ss.str();
}

bool jubeiming::utils::hasParam(const std::vector<std::string>& keys, std::string_view val) {
	return std::find(keys.cbegin(), keys.cend(), val) != keys.cend();
}

bool jubeiming::utils::verifyParam(const crow::request& req, std::string_view val) {
	bool has_value{ jubeiming::utils::hasParam(req.url_params.keys(), val) };
	if (has_value)
	{
		return !std::string(req.url_params.get(val.data())).empty();
	}
	else return false;
}

void jubeiming::utils::replaceStrAll(std::string& str, const std::string& from, const std::string& to) {
	size_t pos = str.find(from);
	while (pos != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos = str.find(from, pos + to.length());
	}
};

void jubeiming::utils::MySQLReplaceDangerousString(std::string& str) {
	replaceStrAll(str, "\"", "\"\"");
	replaceStrAll(str, "\'", "\'\'");
	replaceStrAll(str, "\\", "\\\\");
};

crow::response jubeiming::utils::HandleResponseBody(std::function<std::string(void)> f, const std::string& content_type) {
	crow::response response;
	response.set_header("Content-Type", content_type);
	try {
		response.write(f());
		response.code = 200;
		return response;
	}
	catch (const HTTPException& except) {
		response.write(HTTPUtil::getSimpleJsonResult(except.getCode(), except.getMessage(), except.getJson(), except.getStatus()).dump(2));
		response.code = except.getCode();
	}
	catch (const json::out_of_range& except) {
		response.write(HTTPUtil::getSimpleJsonResult(400, except.what()).dump(2));
		response.code = 400;
	}
	catch (const std::exception& except) {
		response.write(HTTPUtil::getSimpleJsonResult(500, except.what()).dump(2));
		response.code = 500;
	}
	response.set_header("Content-Type", "application/json");
	return response;
}

crow::response jubeiming::utils::HandleResponseBody(std::function<boost::cobalt::task<std::string>(void)> task, const std::string& content_type) {
	crow::response response;
	response.set_header("Content-Type", content_type);
	try {
		boost::asio::io_context ctx{ BOOST_ASIO_CONCURRENCY_HINT_1 };
		auto f = boost::cobalt::spawn(ctx, task(), boost::asio::use_future);
		ctx.run();

		response.write(f.get());
		response.code = 200;
		return response;
	}
	catch (const HTTPException& except) {
		response.write(HTTPUtil::getSimpleJsonResult(except.getCode(), except.getMessage(), except.getJson(), except.getStatus()).dump(2));
		response.code = except.getCode();
	}
	catch (const json::out_of_range& except) {
		response.write(HTTPUtil::getSimpleJsonResult(400, except.what()).dump(2));
		response.code = 400;
	}
	catch (const std::exception& except) {
		response.write(HTTPUtil::getSimpleJsonResult(500, except.what()).dump(2));
		response.code = 500;
	}
	response.set_header("Content-Type", "application/json");
	return response;
}

std::string jubeiming::utils::ParamsString(const std::map<std::string, std::string>& params) {
	std::string url_prams = "";
	for (const auto& it : params) {
		url_prams += it.first + "=" + it.second + "&";
	}
	url_prams.pop_back();
	return url_prams;
};

std::string jubeiming::utils::UUIDGenerator(const jubeiming::define::UuidInfo& info)
{
	snowflake_t::twepoch = info.snowflakeTwepoch;
	snowflake_t snowflakeUUID;
	snowflakeUUID.init(info.snowflakeWorkerid, info.snowflakeDatacenterid);
	auto id = snowflakeUUID.nextid();
	Poco::UUIDGenerator uuidGenerator;
	std::unique_ptr<Poco::DigestEngine> engine{ new Poco::SHA1Engine() };
	Poco::UUID uuid{ uuidGenerator.createFromName(Poco::UUID(info.uuidNamespace), std::to_string(id), *engine, Poco::UUID::UUID_NAME_BASED_SHA1) };
	engine.reset();
	return uuid.toString();
}

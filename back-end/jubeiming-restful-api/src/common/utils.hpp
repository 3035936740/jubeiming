/*
 * @File	  : utils.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2024/06/25 13:55
 * @Introduce : 一些好用的工具
*/

#pragma once

#include <vector>
#include <string_view>
#include <limits>
#include <chrono>
#include <unordered_set>
#include "boost/cobalt.hpp"
#include "crow/http_response.h"
#include "crow/http_request.h"
#include "define.hpp"

#ifndef JUBEIMING_UTILS_HPP
#define JUBEIMING_UTILS_HPP

namespace jubeiming::utils {
	// 计时器
	class Timer {
	private:
		std::chrono::high_resolution_clock::time_point start_time;
		std::chrono::high_resolution_clock::time_point end_time;
	public:
		void start();
		void stop();
		double elapsedMilliseconds() const;
		double elapsedSeconds() const;

		template <typename T>
		auto elapsed() const {
			return std::chrono::duration_cast<T>(end_time - start_time).count();
		};
	};

	/// <summary>
	/// 字符串移除
	/// </summary>
	/// <param name="fullString">原字符串</param>
	/// <param name="substring">需要移除的子字符串</param>
	/// <returns>处理完成的字符串</returns>
	std::string strRemove(const std::string&, const std::string&);

	/// <summary>
	/// 分割字符串
	/// </summary>
	/// <param name="str">需要分割的字符串</param>
	/// <param name="delimiter">以xxx分割</param>
	/// <returns>分割完成的字符串</returns>
	std::vector<std::string> strSplit(const std::string&, std::string_view);

	/// <summary>
	/// 生成随机字符串
	/// </summary>
	/// <param name="length">参数1为长度</param>
	/// <param name="charset">参数2为字符串内容</param>
	/// <returns>生成的字符串</returns>
	std::string generateRandom(std::size_t = 32, const std::string& = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	/// <summary>
	/// 保留浮点数后N位
	/// </summary>
	/// <param name="f">数字</param>
	/// <param name="n">保留多少位</param>
	/// <returns>保留小数后的字符串</returns>
	std::string retainDecimalPlaces(double, int = 2);

	/// <summary>
	/// 判断vector<string>是否存在特定值
	/// </summary>
	/// <param name="keys">crow param的列表</param>
	/// <param name="val">是否含有特定字符串</param>
	/// <returns>true为存在,false为不存在</returns>
	bool hasParam(const std::vector<std::string>&, std::string_view);

	/// <summary>
	/// 效验request参数
	/// </summary>
	/// <param name="req">req丢进去就完事了</param>
	/// <param name="val">是否存在的字符串</param>
	/// <returns>true返回长度不为0的内容</returns>
	bool verifyParam(const crow::request&, std::string_view);

	/// <summary>
	/// 字符串替换工具
	/// </summary>
	/// <param name="str">原字符串</param>
	/// <param name="from">需要替换的字符串</param>
	/// <param name="to">替换的字符</param>
	/// 将str字符串中的from替换成to
	void replaceStrAll(std::string& str, const std::string&, const std::string&);

	/// <summary>
	/// HTTP响应体获取
	/// </summary>
	/// <param name="f">返回string的字符串函数</param>
	/// <param name="content_type">response的类型</param>
	/// <returns>获取到的crow response</returns>
	crow::response HandleResponseBody(std::function<std::string(void)>, const std::string& = "application/json");

	crow::response HandleResponseBody(std::function<boost::cobalt::task<std::string>(void)>, const std::string & = "application/json");

	/// <summary>
	/// get请求参数包装器
	/// </summary>
	/// <param name="params">包装的内容</param>
	/// <returns>包赚完成的请求参数</returns>
	std::string ParamsString(const std::map<std::string, std::string>&);

	/// <summary>
	/// 替换mysql中危险的字符串尽可能的防止sql注入
	/// </summary>
	/// <param name="str">替换的字符串</param>
	void MySQLReplaceDangerousString(std::string&);

	/// <summary>
	/// 字符串替换工具
	/// </summary>
	/// <param name="x">需要比较的1号浮点数</param>
	/// <param name="y">需要比较的2号浮点数</param>
	/// <param name="accuracy">比较精确度</param>
	/// <returns>0等于,1大于,-1小于</returns>
	template <typename T = const double>
	constexpr inline std::int8_t compareDecimal(T x1, T x2, T accuracy = 1e-5) {
		constexpr std::int8_t BIG{ 1 }, SMALL{ -1 }, EQUAL{ 0 };
		T diff{ std::abs(x1 - x2) };
		if (diff <= accuracy) {
			return EQUAL;
		}
		else if (x1 > x2) {
			return BIG;
		}
		else {
			return SMALL;
		}
		return 0;
	}

#include "snowflake.hpp"

	namespace {
		inline constexpr const int64_t __TWEPOCH__{ -1L };
		using snowflake_t = snowflake<__TWEPOCH__, std::mutex>;
	}

	/// <summary>
	/// UUID生成器
	/// </summary>
	/// <param name="info">需要uuid相关数据</param>
	/// <returns>生成的uuid字符串</returns>
	std::string UUIDGenerator(const jubeiming::define::UuidInfo&);
}

#endif // JUBEIMING_UTILS_HPP
/*
 * @File	  : log.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2024/06/24 17:06
 * @Introduce : 日志类
*/

#pragma once

#include <type_traits>
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"

#ifndef JUBEIMING_LOG_HPP
#define JUBEIMING_LOG_HPP

namespace jubeiming {
	class Log {
	public:
		enum class level;
	private:
		std::shared_ptr<spdlog::logger> mLogger{ nullptr };
		std::string mLoggerName, mFilename;
		int mHour, mMinute;

		level mLevel;
		spdlog::level::level_enum mSetLogLevel(level);
		void mLoadLogger();
	public:
		enum class level {
			Trace,
			Debug = spdlog::level::debug,
			Info = spdlog::level::info,
			Warn = spdlog::level::warn,
			Error = spdlog::level::err,
			Critical = spdlog::level::critical,
			Off = spdlog::level::off,
		};

		void setGlobalLogLevel(level);

		explicit Log();
		explicit Log(std::string_view, std::string_view, int, int, level);
		explicit Log(std::string_view, std::string_view, int, int);

		explicit Log(int, int, level = level::Info);

		~Log();

		void info(std::string_view);
		void debug(std::string_view);
		void warn(std::string_view);
		void error(std::string_view);
		void critical(std::string_view);
	};

	using LogLevelType = typename Log::level;
}

#endif // JUBEIMING_LOG_HPP  
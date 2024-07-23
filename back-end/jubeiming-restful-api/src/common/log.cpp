// 日志实现
#include "log.hpp"

spdlog::level::level_enum jubeiming::Log::mSetLogLevel(level level)
{
	spdlog::level::level_enum levelType{};

	switch (level) {
	case level::Trace:
		levelType = spdlog::level::trace;
		break;
	case level::Debug:
		levelType = spdlog::level::debug;
		break;
	case level::Info:
		levelType = spdlog::level::info;
		break;
	case level::Warn:
		levelType = spdlog::level::warn;
		break;
	case level::Error:
		levelType = spdlog::level::err;
		break;
	case level::Critical:
		levelType = spdlog::level::critical;
		break;
	case level::Off:
		levelType = spdlog::level::off;
		break;
	}

	return levelType;
}

void jubeiming::Log::mLoadLogger() {
	mLogger = spdlog::daily_logger_mt(mLoggerName, mFilename, mHour, mMinute);

	mLogger->set_level(mSetLogLevel(mLevel));
	spdlog::set_level(mSetLogLevel(mLevel));
}

void jubeiming::Log::setGlobalLogLevel(level level)
{
	mLogger->set_level(mSetLogLevel(mLevel));
	spdlog::set_level(mSetLogLevel(mLevel));
}

jubeiming::Log::Log(std::string_view loggerName, std::string_view logFilename, int hour, int minute, level level) :
	mLoggerName{ loggerName },
	mFilename{ logFilename },
	mHour{ hour }, mMinute{ minute },
	mLevel{ level } {
	mLoadLogger();
}

jubeiming::Log::Log(std::string_view loggerName, std::string_view logFilename, int hour, int minute) :
	Log(loggerName, logFilename, hour, minute, level::Info) {}

jubeiming::Log::Log() : Log("daily_logger", "logs/daily.txt", 0, 0, level::Info) {}

jubeiming::Log::Log(int hour, int minute, level level) : Log("daily_logger", "logs/daily.txt", hour, minute, level) {}

jubeiming::Log::~Log() {
	mLogger.reset();
}

void jubeiming::Log::info(std::string_view msg)
{
	spdlog::info(msg);
	mLogger->info(msg);
}

void jubeiming::Log::debug(std::string_view msg)
{
	spdlog::debug(msg);
	mLogger->debug(msg);
}

void jubeiming::Log::warn(std::string_view msg)
{
	spdlog::warn(msg);
	mLogger->warn(msg);
}

void jubeiming::Log::error(std::string_view msg)
{
	spdlog::error(msg);
	mLogger->error(msg);
}

void jubeiming::Log::critical(std::string_view msg)
{
	spdlog::critical(msg);
	mLogger->critical(msg);
}
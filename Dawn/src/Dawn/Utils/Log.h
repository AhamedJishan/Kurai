#pragma once

#include <cstdio>
#include <cstdarg>

namespace Dawn::Log
{
	enum class LogLevel
	{
		Info = 0,
		Warn,
		Error
	};

	inline void PrintLogLevelString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Info:  std::printf("\033[37m[INFO] ");  break;
		case LogLevel::Warn:  std::printf("\033[33m[WARN] ");  break;
		case LogLevel::Error: std::printf("\033[31m[ERROR] "); break;
		default: break;
		}
	}

	inline void LogMessage(LogLevel level, const char* format, ...)
	{
		PrintLogLevelString(level);

		va_list args;
		va_start(args, format);
		std::vprintf(format, args);
		va_end(args);

		std::printf("\033[0m\n"); // reset color
	}
}

#ifndef RELEASE
	#define LOG_INFO(...) LogMessage(Dawn::Log::LogLevel::Info, __VA_ARGS__)
	#define LOG_WARN(...) LogMessage(Dawn::Log::LogLevel::Warn, __VA_ARGS__)
#else
	#define LOG_INFO(...)
	#define LOG_WARN(...)
#endif

#define LOG_ERROR(...) LogMessage(Dawn::Log::LogLevel::Error, __VA_ARGS__)
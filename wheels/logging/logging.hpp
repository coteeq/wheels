#pragma once

#include <wheels/core/string_builder.hpp>
#include <wheels/core/source_location.hpp>
#include <fmt/format.h>

// Simple asynchronous logging

namespace wheels {

//////////////////////////////////////////////////////////////////////

enum class LogLevel : int {
  Trace = 1,
  Debug = 2,
  Info = 3,
  Warning = 4,
  Error = 5,
  Critical = 6,
};

//////////////////////////////////////////////////////////////////////

bool LevelAccepted(LogLevel level);

void LogMessage(SourceLocation where, LogLevel level, std::string message);

void FlushPendingLogMessages();

//////////////////////////////////////////////////////////////////////

void LogMessageSimple(std::string message);

//////////////////////////////////////////////////////////////////////

#define _LOG_IMPL(level, ...)                                                 \
  do {                                                                         \
    if (wheels::LevelAccepted(level)) {                                        \
      wheels::LogMessage(WHEELS_HERE, level, fmt::format(__VA_ARGS__)); \
    }                                                                          \
  } while (false);

#define LOG_TRACE(...) _LOG_IMPL(wheels::LogLevel::Trace, __VA_ARGS__)
#define LOG_DEBUG(...) _LOG_IMPL(wheels::LogLevel::Debug, __VA_ARGS__)
#define LOG_INFO(...) _LOG_IMPL(wheels::LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(...) _LOG_IMPL(wheels::LogLevel::Warning, __VA_ARGS__)
#define LOG_ERROR(...) _LOG_IMPL(wheels::LogLevel::Error, __VA_ARGS__)
#define LOG_CRIT(...) _LOG_IMPL(wheels::LogLevel::Critical, __VA_ARGS__)

}  // namespace wheels

#pragma once

#include <wheels/support/string_builder.hpp>
#include <wheels/support/source_location.hpp>

// Simple asynchronous logging

namespace wheels {

//////////////////////////////////////////////////////////////////////

enum class LogLevel : int {
  Trace = 1,
  Debug = 2,
  Info = 3,
  Warning = 4,
  Critical = 5
};

//////////////////////////////////////////////////////////////////////

bool LevelAccepted(LogLevel level);

void LogMessage(SourceLocation where, std::string message);

void FlushPendingLogMessages();

//////////////////////////////////////////////////////////////////////

void LogMessageSimple(std::string message);

//////////////////////////////////////////////////////////////////////

#define LOG_IMPL(level, expr)                                    \
  do {                                                           \
    if (wheels::LevelAccepted(level)) {                           \
      wheels::LogMessage(HERE(), wheels::StringBuilder() << expr); \
    }                                                            \
  } while (false);

#define LOG_DEBUG(expr) LOG_IMPL(wheels::LogLevel::Debug, expr)
#define LOG_TRACE(expr) LOG_IMPL(wheels::LogLevel::Trace, expr)
#define LOG_INFO(expr) LOG_IMPL(wheels::LogLevel::Info, expr)

// Usage: LOG_SIMPLE("Key " << key << " not found")
#define LOG_SIMPLE(expr) \
  wheels::LogMessageSimple(wheels::StringBuilder() << expr);

}  // namespace wheels

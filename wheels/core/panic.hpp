#pragma once

#include <wheels/core/source_location.hpp>
#include <wheels/core/string_builder.hpp>

namespace wheels {

[[noreturn]] void Panic(const std::string& error, SourceLocation where = SourceLocation::Current());

}  // namespace wheels

// Usage: WHEELS_PANIC("Internal error: " << e.what());

#define WHEELS_PANIC(error)                                                   \
  do {                                                                        \
    ::wheels::Panic(::wheels::StringBuilder() << error, WHEELS_HERE); \
  } while (false)

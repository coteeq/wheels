#pragma once

#include <wheels/support/source_location.hpp>
#include <wheels/support/string_builder.hpp>

namespace detail {
[[noreturn]] void Panic(const std::string& error);
}  // namespace detail

// Print source location and error message to stderr, then abort
// Usage: WHEELS_PANIC("Internal error: " << e.what());

#define WHEELS_PANIC(error)                                                   \
  do {                                                                        \
    ::detail::Panic(wheels::StringBuilder() << WHEELS_HERE << ": " << error); \
  } while (false)

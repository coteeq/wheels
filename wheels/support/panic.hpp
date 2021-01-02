#pragma once

#include <wheels/support/source_location.hpp>
#include <wheels/support/string_builder.hpp>

namespace wheels {

namespace detail {
[[noreturn]] void Panic(const std::string& error);
}  // namespace detail

}  // namespace wheels

// Usage: WHEELS_PANIC("Internal error: " << e.what());

#define WHEELS_PANIC(error)                                   \
  do {                                                        \
    ::wheels::detail::Panic(::wheels::StringBuilder()         \
                            << WHEELS_HERE << ": " << error); \
  } while (false)

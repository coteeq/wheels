#pragma once

#include <wheels/support/compiler.hpp>
#include <wheels/support/panic.hpp>

#define WHEELS_VERIFY(cond, error)                                   \
  do {                                                               \
    if (WHEELS_UNLIKELY(!(cond))) {                                  \
      WHEELS_PANIC("Assertion '" << #cond << "' failed: " << error); \
    }                                                                \
  } while (false)

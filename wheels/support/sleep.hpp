#pragma once

#include <wheels/support/time.hpp>

#include <cstddef>

namespace wheels {

void Sleep(Duration delay);

void SleepSeconds(size_t secs);
void SleepMillis(size_t millis);

void SleepRandomMillis(size_t lo, size_t hi);

}  // namespace twist

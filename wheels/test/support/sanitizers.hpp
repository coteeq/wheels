#pragma once

#include <sanitizer/lsan_interface.h>

namespace wheels {

// >= 1, 1 means no slowdown
int GetSanitizerSlowdown();

}  // namespace wheels

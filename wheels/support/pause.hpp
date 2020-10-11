#pragma once

namespace wheels {

inline void SpinLockPause() {
  asm volatile("pause\n" : : : "memory");
}

}  // namespace wheels

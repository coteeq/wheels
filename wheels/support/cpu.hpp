#pragma once

#include <cstdint>

inline uint64_t GetCpuCycleCount() {
  uint32_t hi, lo;
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
  return (static_cast<uint64_t>(hi) << 32) | lo;
}

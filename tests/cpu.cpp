#include <wheels/support/cpu.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>

TEST_SUITE(Cpu) {
  SIMPLE_TEST(Rdtsc) {
    auto start = wheels::GetCpuCycleCount();

    volatile int total = 0;
    for (size_t i = 0; i < 1000; ++i) {
      total += i;
    }

    auto end = wheels::GetCpuCycleCount();
    std::cout << "Loop cycle count: " << end - start << std::endl;
  }

  SIMPLE_TEST(SpinLockPause) {
    for (size_t i = 0; i < 1005001; ++i) {
      wheels::SpinLockPause();
    }
  }
}

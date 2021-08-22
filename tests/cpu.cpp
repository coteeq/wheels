#include <wheels/support/cpu.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>
#include <random>

TEST_SUITE(Cpu) {
  SIMPLE_TEST(Rdtsc) {
    auto start = wheels::GetCpuCycleCount();

    std::mt19937 twister;

    size_t total = 0;
    for (size_t i = 0; i < 1000; ++i) {
      total += twister();
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

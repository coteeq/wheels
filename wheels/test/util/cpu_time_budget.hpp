#pragma once

#include <wheels/test/util/cpu_timer.hpp>

#include <wheels/test/test_framework.hpp>

#include <chrono>

namespace wheels::test {

class CpuTimeBudgetGuard {
 public:
  explicit CpuTimeBudgetGuard(std::chrono::milliseconds budget)
      : budget_(budget) {}

  std::chrono::nanoseconds Usage() const {
    return timer_.Elapsed();
  }

  ~CpuTimeBudgetGuard() {
    ASSERT_TRUE(timer_.Elapsed() < budget_);
  }

 private:
  std::chrono::milliseconds budget_;
  ProcessCPUTimer timer_;
};

}  // namespace wheels::test

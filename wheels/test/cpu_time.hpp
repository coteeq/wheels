#pragma once

#include <wheels/support/cpu_time.hpp>

#include <wheels/test/test_framework.hpp>

#include <chrono>

namespace wheels::test {

class CpuTimeBudgetGuard {
  using Duration = std::chrono::microseconds;
 public:
  explicit CpuTimeBudgetGuard(Duration budget)
      : budget_(budget) {}

  Duration Usage() const {
    return timer_.Elapsed();
  }

  ~CpuTimeBudgetGuard() {
    ASSERT_TRUE(timer_.Elapsed() < budget_);
  }

 private:
  Duration budget_;
  ProcessCPUTimer timer_;
};

}  // namespace wheels::test

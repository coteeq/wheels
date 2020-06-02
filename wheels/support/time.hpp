#pragma once

#include <chrono>

namespace wheels {

////////////////////////////////////////////////////////////////////////////////

using Duration = std::chrono::nanoseconds;

////////////////////////////////////////////////////////////////////////////////

class StopWatch {
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<Clock>;

 public:
  StopWatch() : start_(Now()) {
  }

  Duration Elapsed() const {
    return Now() - start_;
  }

  Duration Restart() {
    auto elapsed = Elapsed();
    start_ = Now();
    return elapsed;
  }

 private:
  static TimePoint Now() {
    return Clock::now();
  }

 private:
  TimePoint start_;
};

}  // namespace wheels

#pragma once

#include <wheels/test/test_options.hpp>

#include <wheels/support/time.hpp>

#include <atomic>
#include <thread>
#include <chrono>

namespace wheels::test {

class TestTimeLimitWatcher {
  using Clock = std::chrono::steady_clock;

 public:
  TestTimeLimitWatcher(wheels::Duration time_limit);

  ~TestTimeLimitWatcher() {
    Join();
  }

 private:
  void Watch();
  void Join();

 private:
  Duration time_limit_;
  std::atomic<bool> stop_requested_{false};
  std::thread watcher_thread_;
};

wheels::Duration TestTimeLimit(TestOptions options);

}  // namespace wheels::test

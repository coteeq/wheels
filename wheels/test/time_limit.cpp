#include <wheels/test/time_limit.hpp>

#include <wheels/test/test_framework.hpp>

#include <wheels/test/helpers.hpp>

#include <wheels/support/one_shot_event.hpp>
#include <wheels/support/sanitizers.hpp>

#include <thread>

namespace wheels::test {

#if WHEELS_NO_TEST_TIME_LIMIT

class TestTimeLimitWatcher::Impl {
 public:
  Impl(Duration /* timeout */) {
  }
};

#else

class TestTimeLimitWatcher::Impl {
 public:
  Impl(Duration timeout)
      : timeout_(timeout), watcher_thread_(&Impl::Watch, this) {
  }

  ~Impl() {
    stop_requested_.Set();
    watcher_thread_.join();
  }

 private:
  void Watch() {
    if (!stop_requested_.TimedWait(timeout_)) {
      FAIL_TEST("Time limit exceeded - " << ToSeconds(timeout_) << " seconds");
    }
  }

 private:
  Duration timeout_;
  wheels::OneShotEvent stop_requested_;
  std::thread watcher_thread_;
};

#endif

TestTimeLimitWatcher::TestTimeLimitWatcher(Duration timeout)
    : pimpl_(std::make_unique<TestTimeLimitWatcher::Impl>(timeout)) {
}

TestTimeLimitWatcher::~TestTimeLimitWatcher() {
}

wheels::Duration TestTimeLimit(TestOptions options) {
  auto time_limit = options.time_limit;
  if (options.adapt_time_limit_to_sanitizer_) {
    time_limit *= GetSanitizerSlowdown();
  }
  return time_limit;
}

}  // namespace wheels::test

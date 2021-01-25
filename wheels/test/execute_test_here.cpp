#include <wheels/test/execute_test_here.hpp>

#include <wheels/test/test_framework.hpp>
#include <wheels/test/time_limit.hpp>
#include <wheels/test/execute_test.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/assert.hpp>
#include <wheels/support/panic.hpp>
#include <wheels/support/exception.hpp>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

////////////////////////////////////////////////////////////////////////////////

class TestRunner;

static TestRunner* current_runner{nullptr};

struct TestRunnerScope {
  TestRunnerScope(TestRunner* r) {
    current_runner = r;
  }

  ~TestRunnerScope() {
    current_runner = nullptr;
  }
};

TestRunner& AccessTestRunner() {
  WHEELS_VERIFY(current_runner != nullptr, "Not in test context");
  return *current_runner;
}

////////////////////////////////////////////////////////////////////////////////

class TestRunner {
 public:
  TestRunner(ITestPtr test)
      : test_(std::move(test)) {
  }

  void Run() {
    TestRunnerScope scope(this);

    start_time_ = Clock::now();

    time_limit_ = TestTimeLimit(test_->Options());

    TestTimeLimitWatcher time_limit_watcher(time_limit_);

    try {
      test_->Run();
    } catch (...) {
      FailTestByException();
    }
  }

  const ITestPtr& Test() const {
    return test_;
  }

  Duration TimeLimit() const {
    return time_limit_;
  }

  Duration TimeElapsed() const {
    return Clock::now() - start_time_;
  }

  Duration TimeLeft() const {
    auto elapsed = TimeElapsed();
    if (elapsed < time_limit_) {
      return time_limit_ - elapsed;
    } else {
      return Duration{0};
    }
  }

 private:
  ITestPtr test_;
  TimePoint start_time_;
  Duration time_limit_;
};

void ExecuteTestHere(const ITestPtr& test) {
  TestRunner runner{test};
  runner.Run();
}

////////////////////////////////////////////////////////////////////////////////

// Current test

Duration TestTimeLimit() {
  return AccessTestRunner().TimeLimit();
}

Duration TestTimeLeft() {
  return AccessTestRunner().TimeLeft();
}

}  // namespace wheels::test

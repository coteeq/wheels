#include <wheels/test/here.hpp>

#include <wheels/test/test_framework.hpp>
#include <wheels/test/time_limit.hpp>
#include <wheels/test/run_test.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/assert.hpp>
#include <wheels/support/panic.hpp>
#include <wheels/support/exception.hpp>

#include <any>
#include <map>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

////////////////////////////////////////////////////////////////////////////////

class TestRunner;

static TestRunner* current_runner{nullptr};

struct TestRunnerScope {
  TestRunnerScope(TestRunner* runner) {
    current_runner = runner;
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
  TestRunner(ITestPtr test, const GlobalOptions& options)
      : options_(options), test_(std::move(test)) {
  }

  void Run() {
    TestRunnerScope scope(this);

    start_time_ = Clock::now();

    InitTimeLimit();
    TestTimeLimitWatcher time_limit_watcher(time_limit_);

    SetContext("test_hash", ComputeTestHash());

    try {
      test_->Run();
    } catch (...) {
      FailTestByException();
    }
  }

  const ITestPtr& Test() const {
    return test_;
  }

  std::string FullTestName() const {
    return test_->Suite() + ":" + test_->Name();
  }

  size_t ComputeTestHash() const {
    return std::hash<std::string>()(FullTestName());
  }

  void InitTimeLimit() {
    static const auto kInfTimeLimit = 100500s;

    if (options_.disable_time_limits) {
      time_limit_ = kInfTimeLimit;
    } else {
      time_limit_ = TestTimeLimit(test_->Options());
    }
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

  std::any GetContext(const std::string& key) {
    if (auto it = context_.find(key); it != context_.end()) {
      return it->second;
    }
    return {};
  }

  void SetContext(const std::string& key, std::any value) {
    context_.insert_or_assign(key, value);
  }

 private:
  GlobalOptions options_;
  ITestPtr test_;
  Duration time_limit_;
  TimePoint start_time_;
  std::map<std::string, std::any> context_;
};

void RunTestHere(const ITestPtr& test, const GlobalOptions& options) {
  TestRunner runner{test, options};
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

size_t TestHash() {
  return AccessTestRunner().ComputeTestHash();
}

std::any GetContextImpl(const std::string& key) {
  return AccessTestRunner().GetContext(key);
}

void SetContextImpl(const std::string& key, std::any value) {
  AccessTestRunner().SetContext(key, std::move(value));
}

}  // namespace wheels::test

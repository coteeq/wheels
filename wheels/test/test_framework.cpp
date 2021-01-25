#include <wheels/test/test_framework.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/exception.hpp>
#include <wheels/support/one_shot_event.hpp>
#include <wheels/support/panic.hpp>
#include <wheels/support/singleton.hpp>
#include <wheels/support/string_builder.hpp>

#include <wheels/test/console_reporter.hpp>
#include <wheels/test/execute_test.hpp>
#include <wheels/test/fail_handler.hpp>
#include <wheels/test/helpers.hpp>

#include <wheels/logging/logging.hpp>

#include <chrono>
#include <cstdlib>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

using wheels::Duration;

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

ITestReporterPtr GetReporter() {
  return GetConsoleReporter();
}

////////////////////////////////////////////////////////////////////////////////

class AbortOnFailHandler : public ITestFailHandler {
 public:
  void Fail(ITestPtr test, const std::string& error) override {
    wheels::FlushPendingLogMessages();
    GetReporter()->TestFailed(test, error);
    std::abort();
  }
};

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& out, const AssertionFailure& failure) {
  out << "Assertion '" << failure.condition_ << "' failed at "
      << failure.where_;
  auto description = failure.GetDescription();
  if (!description.empty()) {
    out << ": " << description;
  }
  return out;
}

////////////////////////////////////////////////////////////////////////////////

struct TestContext {
 public:
  void SetCurrentTest(ITestPtr test) {
    current_ = std::move(test);
  }

  void Reset() {
    current_.reset();
  }

  const ITestPtr& GetCurrentTest() {
    if (!current_) {
      std::terminate();
    }
    return current_;
  }

 private:
  ITestPtr current_{};
};

struct TestContextGuard {
  TestContextGuard(ITestPtr test) {
    Singleton<TestContext>()->SetCurrentTest(std::move(test));
  }

  ~TestContextGuard() {
    Singleton<TestContext>()->Reset();
  }
};

const ITestPtr& CurrentTest() {
  return Singleton<TestContext>()->GetCurrentTest();
}

////////////////////////////////////////////////////////////////////////////////

void FailTest(const std::string& error_message) {
  static std::mutex mutex;

  std::lock_guard<std::mutex> locked(mutex);

  GetTestFailHandler()->Fail(CurrentTest(), error_message);
}

void FailTestByAssert(const AssertionFailure& assert_failure) {
  FailTest(assert_failure.ToString());
}

static std::string FormatCurrentExceptionMessage() {
  return StringBuilder() << "Uncaught exception in thread "
                         << std::this_thread::get_id() << ": "
                         << CurrentExceptionMessage();
}

void FailTestByException() {
  FailTest(FormatCurrentExceptionMessage());
}

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

static void DisableStdoutBuffering() {
  std::cout.setf(std::ios::unitbuf);
}

static void PrintCompilerVersion() {
  std::cout << "Compiler: " __VERSION__ << std::endl;
}

static void PrintSanitizerInfo() {
#if __has_feature(address_sanitizer)
  std::cout << "Sanitizer: Address "
               "(https://clang.llvm.org/docs/AddressSanitizer.html)"
            << std::endl;
#elif __has_feature(thread_sanitizer)
  std::cout
      << "Sanitizer: Thread (https://clang.llvm.org/docs/ThreadSanitizer.html)"
      << std::endl;
#else
  // Do not care
#endif

  int sanitizer_slowdown = GetSanitizerSlowdown();
  if (sanitizer_slowdown != 1) {
    std::cout << "Expected slowdown introduced by sanitizer: x"
              << sanitizer_slowdown << std::endl;
  }
}

static void PrintTestFrameworkMode() {
  if (UseForks()) {
    std::cout << "Use forks: YES, run tests in subprocesses (set CMake option "
                 "WHEELS_FORK_TESTS=OFF to disable forks)"
              << std::endl;
  } else {
    std::cout << "Use forks: NO" << std::endl;
  }
}

static void RunTest(ITestPtr test, ITestReporterPtr reporter) {
  TestContextGuard ctx_installer(test);

  reporter->TestStarted(test);

  wheels::StopWatch stop_watch;

  try {
    ExecuteTest(test);
  } catch (...) {
    WHEELS_PANIC(
        "Test framework internal error: " << CurrentExceptionMessage());
  }

  reporter->TestPassed(test, stop_watch.Elapsed());
}

TestList FilterTests(const TestList& tests, ITestFilterPtr filter) {
  TestList selected_tests;
  for (const auto& test : tests) {
    if (filter->Accept(test)) {
      selected_tests.push_back(test);
    }
  }
  return selected_tests;
}

TestList FilterTestSuites(const TestList& tests,
                          std::vector<std::string> suites) {
  TestList result;
  for (const auto& suite : suites) {
    for (const auto& test : tests) {
      if (test->Suite() == suite) {
        result.push_back(test);
      }
    }
  }
  return result;
}

void RunTests(const TestList& tests) {
  DisableStdoutBuffering();
  PrintCompilerVersion();
  PrintSanitizerInfo();
  PrintTestFrameworkMode();

  InstallTestFailHandler(std::make_shared<AbortOnFailHandler>());

  auto reporter = GetReporter();

  wheels::StopWatch stop_watch;

  for (auto&& test : tests) {
    RunTest(test, reporter);
  }

  reporter->AllTestsPassed(tests.size(), stop_watch.Elapsed());
}

}  // namespace wheels::test

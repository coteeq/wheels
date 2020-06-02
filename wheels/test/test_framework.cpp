#include <wheels/test/test_framework.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/one_shot_event.hpp>
#include <wheels/support/panic.hpp>
#include <wheels/support/singleton.hpp>
#include <wheels/support/string_builder.hpp>
#include <wheels/support/terminal.hpp>

#include <wheels/test/emoji.hpp>
#include <wheels/test/execute_test.hpp>
#include <wheels/test/reporter.hpp>
#include <wheels/test/fail_handler.hpp>

#include <wheels/logging/logging.hpp>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

using wheels::Duration;

////////////////////////////////////////////////////////////////////////////////

static double ToSeconds(wheels::Duration elapsed) {
  return std::chrono::duration<double>(elapsed).count();
}

////////////////////////////////////////////////////////////////////////////////

class ConsoleTestReporter : public ITestReporter {
 public:
  void TestStarted(const ITestPtr& test) override {
    PrintSeparatorLine();
    std::cout << "Test " << CYAN(test->Describe()) << " running..."
              << std::endl;
  }

  void TestFailed(const ITestPtr& test, const std::string& error) override {
    std::cout << "Test " << test->Describe() << RED(" FAILED ")
              << GetFailEmoji() << ": " << error << std::endl
              << std::flush;
  }

  void TestPassed(const ITestPtr& test, wheels::Duration elapsed) override {
    WHEELS_UNUSED(test);
    std::cout << GREEN("PASSED") << " (" << std::fixed << std::setprecision(3)
              << ToSeconds(elapsed) << " seconds)" << std::endl;
  }

  void AllTestsPassed(size_t test_count, wheels::Duration elapsed) override {
    if (test_count == 0) {
      std::cout << "NO TESTS TO RUN..." << std::endl;
      return;
    }

    PrintSeparatorLine();

    if (test_count > 1) {
      std::cout << GREEN("ALL " << test_count << " TESTS PASSED!");
    } else {
      std::cout << GREEN("1 TEST PASSED!");
    }
    std::cout << " (total time: " << ToSeconds(elapsed) << " seconds)"
              << std::endl
              << GetSuccessEmoji() << std::endl
              << std::flush;
  }

 private:
  static void PrintSeparatorLine() {
    static const std::string kSeparatorLine(80, '-');
    std::cout << kSeparatorLine << std::endl;
  }
};

ITestReporterPtr GetReporter() {
  static auto reporter = std::make_shared<ConsoleTestReporter>();
  return reporter;
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

std::ostream& operator<<(std::ostream& out, const AssertionError& error) {
  out << "Assertion '" << error.cond_ << "' failed at " << error.where_;
  auto description = error.GetDescription();
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

  ITestPtr GetCurrentTest() {
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

static ITestPtr CurrentTest() {
  return Singleton<TestContext>()->GetCurrentTest();
}

////////////////////////////////////////////////////////////////////////////////

void FailTest(const std::string& error_message) {
  static std::mutex mutex;

  std::lock_guard<std::mutex> locked(mutex);

  GetTestFailHandler()->Fail(CurrentTest(), error_message);
}

void FailTestByAssert(const AssertionError& assert_error) {
  FailTest(assert_error.ToString());
}

static std::string CurrentExceptionMessage() {
  try {
    throw;
  } catch (const std::exception& e) {
    return e.what();
  } catch (...) {
    return "wild exception...";
  }
}

static std::string FormatCurrentExceptionMessage() {
  std::ostringstream out;
  out << "Uncaught exception in thread " << std::this_thread::get_id() << ": "
      << CurrentExceptionMessage();
  return out.str();
}

void FailTestByException() {
  FailTest(FormatCurrentExceptionMessage());
}

////////////////////////////////////////////////////////////////////////////////

class TestRepository {
 public:
  void Add(ITestPtr test) {
    int p = test->Priority();
    tests_[p].push_back(std::move(test));
  }

  TestList ListAll() const {
    TestList all;
    for (const auto& [p, tests] : tests_) {
      for (const auto& t : tests) {
        all.push_back(t);
      }
    }
    return all;
  }

 private:
  // priority -> test list
  std::map<int, TestList> tests_;
};

void RegisterTest(ITestPtr test) {
  Singleton<TestRepository>()->Add(std::move(test));
}

TestList ListAllTests() {
  return Singleton<TestRepository>()->ListAll();
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

static void RunSingleTest(ITestPtr test, ITestReporterPtr reporter) {
  TestContextGuard ctx_installer(test);

  reporter->TestStarted(test);

  wheels::Timer timer;

  try {
    ExecuteTest(test);
  } catch (...) {
    WHEELS_PANIC("Test framework internal error: " << CurrentExceptionMessage());
  }

  reporter->TestPassed(test, timer.Elapsed());
}

TestList FilterTests(const TestList& tests, ITestFilterPtr filter) {
  TestList chosen_tests;
  for (const auto& test : tests) {
    if (filter->Accept(test)) {
      chosen_tests.push_back(test);
    }
  }
  return chosen_tests;
}

void RunTests(const TestList& tests) {
  DisableStdoutBuffering();
  PrintCompilerVersion();

  InstallTestFailHandler(std::make_shared<AbortOnFailHandler>());

  auto reporter = GetReporter();

  wheels::Timer timer;

  for (auto&& test : tests) {
    RunSingleTest(test, reporter);
  }

  reporter->AllTestsPassed(tests.size(), timer.Elapsed());
}

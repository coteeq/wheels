#include <wheels/test/execute_test.hpp>

#include <wheels/test/fail_handler.hpp>
#include <wheels/test/test_framework.hpp>
#include <wheels/test/helpers.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/sync_output.hpp>

#include <wheels/logging/logging.hpp>

#if UNIX && WHEELS_FORK_TESTS

#include <sys/types.h>
#include <unistd.h>

#include <wheels/support/fork.hpp>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

// Execute test in forked subprocess

class ForkedTestFailHandler : public ITestFailHandler {
 public:
  void Fail(ITestPtr test, const std::string& error) override {
    WHEELS_UNUSED(test);
    FlushPendingLogMessages();
    std::cerr << error << std::endl << std::flush;
    std::abort();
  }
};

static void InstallForkedTestFailHandler() {
  InstallTestFailHandler(std::make_shared<ForkedTestFailHandler>());
}

static void ExecuteTestInForkedProcess(ITestPtr test) {
  std::cout << "Executed subprocess with pid = " << getpid() << std::endl;

  InstallForkedTestFailHandler();

  try {
    test->Run();
  } catch (...) {
    FailTestByException();
  }

  FlushPendingLogMessages();
}

class PrintStdoutConsumer : public IByteStreamConsumer {
 public:
  void Consume(const char* buf, size_t length) override {
    std::cout.write(buf, length);
    bytes_consumed_ += length;
  }

  void Eof() override {
    if (bytes_consumed_ > 0) {
      std::cout << std::endl;
    }
  }

 private:
  size_t bytes_consumed_ = 0;
};

static void ExecuteTestWithFork(ITestPtr test) {
  auto execute_test = [test]() { ExecuteTestInForkedProcess(test); };

  auto result = ExecuteWithFork(
      execute_test, std::make_unique<PrintStdoutConsumer>(), nullptr);

  // Process result

  const auto& stderr = result.GetStderr();

  int exit_code;
  if (result.Exited(exit_code)) {
    if (exit_code != 0) {
      FAIL_TEST("Test subprocess terminated with non-zero exit code: "
                << exit_code
                << ", stderr: " << FormatStderrForErrorMessage(stderr));
    }
  }

  int signal;
  if (result.KilledBySignal(signal)) {
    if (stderr.empty()) {
      FAIL_TEST("Test subprocess terminated by signal: " << signal);
    } else {
      FAIL_TEST("Test subprocess terminated by signal "
                << signal
                << ", stderr: " << FormatStderrForErrorMessage(stderr));
    }
  }

  if (!stderr.empty()) {
    FAIL_TEST("Test produced stderr: " << FormatStderrForErrorMessage(stderr));
  }

  // Test completed!
}

void ExecuteTest(ITestPtr test) {
  ExecuteTestWithFork(std::move(test));
}

}  // namespace wheels::test

#else

////////////////////////////////////////////////////////////////////////////////

namespace wheels::test {

// Execute test locally

static void ExecuteTestInCurrentProcess(ITestPtr test) {
  test->Run();
}

void ExecuteTest(ITestPtr test) {
  ExecuteTestInCurrentProcess(std::move(test));
}

}  // namespace wheels::test

#endif

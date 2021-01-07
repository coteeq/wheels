#include <wheels/test/console_reporter.hpp>

#include <wheels/test/emoji.hpp>
#include <wheels/test/helpers.hpp>

#include <wheels/support/terminal.hpp>

#include <iostream>

using namespace std::chrono_literals;

namespace wheels::test {

class ConsoleTestReporter : public ITestReporter {
 public:
  void TestStarted(const ITestPtr& test) override {
    PrintSeparatorLine();
    std::cout << "Test '" << MAGENTA(test->Describe()) << "' from suite '"
              << CYAN(test->Suite()) << "' is running..." << std::endl;
  }

  void TestFailed(const ITestPtr& test, const std::string& error) override {
    std::cout << "Test " << test->Describe() << RED(" FAILED ")
              << GetFailEmoji() << ": " << error << std::endl
              << std::flush;
  }

  void TestPassed(const ITestPtr& /*test*/, wheels::Duration elapsed) override {
    static const auto kSignificantTestTime = 10ms;

    std::cout << GREEN("PASSED");
    if (elapsed > kSignificantTestTime) {
      std::cout << " (" << FormatSeconds(elapsed, 2) << " seconds)";
    }
    std::cout << std::endl;
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
    std::cout << " (total time: " << FormatSeconds(elapsed, 2) << " seconds)"
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

////////////////////////////////////////////////////////////////////////////////

ITestReporterPtr GetConsoleReporter() {
  static const auto instance = std::make_shared<ConsoleTestReporter>();
  return instance;
}

}  // namespace wheels::test

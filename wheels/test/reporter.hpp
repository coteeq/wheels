#pragma once

#include <wheels/test/test.hpp>

#include <wheels/support/time.hpp>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

class ITestReporter {
 public:
  virtual ~ITestReporter() = default;

  virtual void TestStarted(const ITestPtr& test) = 0;

  virtual void TestFailed(const ITestPtr& test, const std::string& error) = 0;

  virtual void TestPassed(const ITestPtr& test, wheels::Duration elapsed) = 0;

  virtual void AllTestsPassed(size_t test_count, wheels::Duration elapsed) = 0;
};

using ITestReporterPtr = std::shared_ptr<ITestReporter>;

}  // namespace wheels::test

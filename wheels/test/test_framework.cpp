#include <wheels/test/test_framework.hpp>

#include <wheels/support/exception.hpp>
#include <wheels/support/string_builder.hpp>

#include <wheels/test/runtime.hpp>

#include <cstdlib>
#include <string>
#include <thread>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

const ITestPtr& CurrentTest() {
  return Runtime::Access().CurrentTest();
}

////////////////////////////////////////////////////////////////////////////////

void FailTest(const std::string& reason) {
  Runtime::Access().FailCurrentTest(reason);
}

void FailTestByAssert(const AssertionFailure& failure) {
  FailTest(failure.ToString());
}

static std::string FormatCurrentExceptionMessage() {
  return StringBuilder() << "Uncaught exception in thread "
                         << std::this_thread::get_id() << ": "
                         << CurrentExceptionMessage();
}

void FailTestByException() {
  FailTest(FormatCurrentExceptionMessage());
}

}  // namespace wheels::test

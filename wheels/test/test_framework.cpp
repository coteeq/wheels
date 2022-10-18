#include <wheels/test/test_framework.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/exception.hpp>
#include <wheels/support/panic.hpp>
#include <wheels/support/string_builder.hpp>

#include <wheels/test/helpers.hpp>

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

void FailTest(const std::string& error_message) {
  Runtime::Access().FailCurrentTest(error_message);
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

}  // namespace wheels::test

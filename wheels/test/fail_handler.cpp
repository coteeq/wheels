#include <wheels/test/fail_handler.hpp>

#include <wheels/support/panic.hpp>

namespace wheels {

static ITestFailHandlerPtr handler = nullptr;

void InstallTestFailHandler(ITestFailHandlerPtr h) {
  handler = h;
}

ITestFailHandlerPtr GetTestFailHandler() {
  if (!handler) {
    WHEELS_PANIC("Test fail handler not installed");
  }
  return handler;
}

}  // namespace wheels

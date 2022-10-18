#include <wheels/test/fail_handler.hpp>

#include <wheels/test/runtime.hpp>

namespace wheels::test {

void InstallTestFailHandler(ITestFailHandlerPtr handler) {
  Runtime::Access().InstallFailHandler(std::move(handler));
}

ITestFailHandlerPtr GetTestFailHandler() {
  return Runtime::Access().GetFailHandler();
}

}  // namespace wheels::test

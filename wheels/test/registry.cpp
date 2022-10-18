#include <wheels/test/registry.hpp>

#include <wheels/test/runtime.hpp>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

void RegisterTest(ITestPtr test) {
  Runtime::Access().Register(std::move(test));
}

}  // namespace wheels::test

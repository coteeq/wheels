#include <wheels/test/execute_test.hpp>

#if UNIX && WHEELS_FORK_TESTS

#include <wheels/test/fork.hpp>

namespace wheels::test {

void ExecuteTest(ITestPtr test) {
  ExecuteTestWithFork(std::move(test));
}

bool UseForks() {
  return true;
}

}  // namespace wheels::test

#else

#include <wheels/test/test_framework.hpp>
#include <wheels/test/run_test_here.hpp>

namespace wheels::test {

void ExecuteTest(ITestPtr test) {
  ExecuteTestHere(test);
}

bool UseForks() {
  return false;
}

}  // namespace wheels::test

#endif

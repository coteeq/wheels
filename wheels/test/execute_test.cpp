#include <wheels/test/execute_test.hpp>

#if UNIX && WHEELS_FORK_TESTS

#include <wheels/test/fork.hpp>

namespace wheels::test {

void ExecuteTest(ITestPtr test) {
  ExecuteTestWithFork(std::move(test));
}

}  // namespace wheels::test

#else

#include <wheels/test/test_framework.hpp>

namespace wheels::test {

// Execute test locally

void ExecuteTestHere() {
  try {
    test->Run();
  } catch (...) {
    FailTestByException();
  }
}

void ExecuteTest(ITestPtr test) {
  ExecuteTestHere();
}

}  // namespace wheels::test

#endif

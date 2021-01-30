#include <wheels/test/execute_test.hpp>

#include <wheels/test/execute_test_here.hpp>
#include <wheels/test/fork.hpp>

namespace wheels::test {

void ExecuteTest(ITestPtr test, const Options& options) {
  if (options.forks || test->Options().force_fork_) {
    ExecuteTestWithFork(std::move(test), options);
  } else {
    ExecuteTestHere(std::move(test), options);
  }
}

}  // namespace wheels::test

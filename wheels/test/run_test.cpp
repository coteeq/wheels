#include <wheels/test/run_test.hpp>

#include <wheels/test/here.hpp>
#include <wheels/test/fork.hpp>

namespace wheels::test {

void RunTest(ITestPtr test, const GlobalOptions& options) {
  if (options.forks || test->Options().force_fork_) {
    RunTestWithFork(std::move(test), options);
  } else {
    RunTestHere(std::move(test), options);
  }
}

}  // namespace wheels::test

#pragma once

#include <wheels/test/test.hpp>

namespace wheels::test {

void ExecuteTestWithFork(ITestPtr test, const Options& options);

}  // namespace wheels::test

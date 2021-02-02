#pragma once

#include <wheels/test/test.hpp>

namespace wheels::test {

void ExecuteTestWithFork(ITestPtr test, const GlobalOptions& options);

}  // namespace wheels::test

#pragma once

#include <wheels/test/test.hpp>

namespace wheels::test {

void ExecuteTest(ITestPtr test);

bool UseForks();

}  // namespace wheels::test

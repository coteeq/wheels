#pragma once

#include <wheels/test/test.hpp>

namespace wheels::test {

void RegisterTest(ITestPtr test);

TestList ListAllTests();

}  // namespace wheels::test

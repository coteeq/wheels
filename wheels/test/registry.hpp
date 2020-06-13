#pragma once

#include <wheels/test/test.hpp>

#include <vector>

namespace wheels::test {

using TestList = std::vector<ITestPtr>;

void RegisterTest(ITestPtr test);
TestList ListAllTests();

}  // namespace wheels::test

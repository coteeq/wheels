#pragma once

#include <wheels/test/test_framework.hpp>

#include <functional>

namespace wheels::test {

void Iterate(std::function<void()> test_routine);

size_t TestIteration();
size_t TestIterationHash();

}  // namespace wheels::test

#define ITERATE_TEST(name, time_budget)                              \
  void IteratedTestRoutine##name();                                  \
  TEST(name, ::wheels::test::TestOptions().TimeLimit(time_budget)) { \
    ::wheels::test::Iterate([]() { IteratedTestRoutine##name(); });  \
  }                                                                  \
  void IteratedTestRoutine##name()

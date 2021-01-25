#pragma once

#include <wheels/test/test_framework.hpp>

#include <functional>

namespace wheels::test {

void Iterate(std::function<void()> test_routine);

}  // namespace wheels::test

#define ITERATE_TEST(name, time_limit) \
  void IteratedTestRoutine##name(); \
  TEST(name, ::wheels::test::TestOptions().TimeLimit(time_limit)) { \
    ::wheels::test::Iterate([]() { \
      IteratedTestRoutine##name(); \
    }); \
  } \
  void IteratedTestRoutine##name()

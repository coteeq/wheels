#pragma once

#include <wheels/support/time.hpp>

using namespace std::chrono_literals;

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

struct TestSuiteOptions {};

////////////////////////////////////////////////////////////////////////////////

struct TestOptions {
  TestOptions() : time_limit(10s) {
  }

  TestOptions TimeLimit(wheels::Duration value) {
    time_limit = value;
    return *this;
  }

  wheels::Duration time_limit;
};

}  // namespace wheels::test

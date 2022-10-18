#pragma once

#include <wheels/test/test.hpp>

#include <chrono>

namespace wheels::test {

// Current test

const ITestPtr& CurrentTest();

// Access only from test routine
// Thread-safe!

std::chrono::milliseconds TestTimeLimit();
std::chrono::milliseconds TestTimeLeft();

// Deterministic
size_t TestHash();

}  // namespace wheels::test

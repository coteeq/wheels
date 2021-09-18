#pragma once

#include <wheels/test/assert_failure.hpp>
#include <wheels/test/test.hpp>
#include <wheels/test/registry.hpp>
#include <wheels/test/filter.hpp>
#include <wheels/test/main.hpp>
#include <wheels/test/test_options.hpp>
#include <wheels/test/context.hpp>

#include <wheels/support/nullptr.hpp>
#include <wheels/support/preprocessor.hpp>
#include <wheels/support/string_builder.hpp>
#include <wheels/support/time.hpp>

#include <iostream>
#include <chrono>
#include <vector>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

void FailTest(const std::string& error_message);
void FailTestByAssert(const AssertionFailure& assert_failure);
void FailTestByException();

////////////////////////////////////////////////////////////////////////////////

#define ASSERT_TRUE(cond)                                      \
  if (!(cond)) {                                               \
    ::wheels::test::FailTestByAssert(ASSERTION_FAILURE(cond)); \
  }

#define ASSERT_TRUE_M(cond, message)                                      \
  if (!(cond)) {                                                          \
    ::wheels::test::FailTestByAssert(ASSERTION_FAILURE(cond) << message); \
  }

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_FALSE_M(cond, message) ASSERT_TRUE_M(!(cond), message)

#define ASSERT_EQ(x, y) ASSERT_TRUE((x) == (y))
#define ASSERT_NE(x, y) ASSERT_TRUE((x) != (y))

// Strict
#define ASSERT_GT(x, y) ASSERT_TRUE((x) > (y))
#define ASSERT_LT(x, y) ASSERT_TRUE((x) < (y))

// Non-strict
#define ASSERT_GE(x, y) ASSERT_TRUE((x) >= (y))
#define ASSERT_LE(x, y) ASSERT_TRUE((x) <= (y))

#define ASSERT_THROW(expr, exception)                                        \
  try {                                                                      \
    expr;                                                                    \
    ASSERT_TRUE_M(false, "Expected exception '" #exception "' not thrown")   \
  } catch (exception & expected) {                                           \
  } catch (...) {                                                            \
    ASSERT_TRUE_M(false,                                                     \
                  "Thrown unexpected exception, expected '" #exception "'"); \
  }

#define FAIL_TEST(error) \
  ::wheels::test::FailTest(::wheels::StringBuilder() << error)

////////////////////////////////////////////////////////////////////////////////

// Test suite provides separate namespace for bunch of simple test functions

#define TEST_SUITE(name)                    \
  namespace TestSuite##name {               \
    std::string GetCurrentTestSuiteName() { \
      return #name;                         \
    }                                       \
  }                                         \
  namespace TestSuite##name

#define TEST(name, options)                                         \
  void TestRoutine##name();                                         \
  struct Test##name : public ::wheels::test::ITest {                \
    std::string Name() const override {                             \
      return #name;                                                 \
    }                                                               \
    std::string Suite() const override {                            \
      return GetCurrentTestSuiteName();                             \
    }                                                               \
    ::wheels::test::TestOptions Options() const override {          \
      return options;                                               \
    }                                                               \
    void Run() override {                                           \
      TestRoutine##name();                                          \
    }                                                               \
  };                                                                \
  struct Test##name##Registrar {                                    \
    Test##name##Registrar() {                                       \
      ::wheels::test::RegisterTest(std::make_shared<Test##name>()); \
    }                                                               \
  };                                                                \
  static Test##name##Registrar test_##name##_registrar_;            \
  void TestRoutine##name()

#define SIMPLE_TEST(name) TEST(name, ::wheels::test::TestOptions{})

////////////////////////////////////////////////////////////////////////////////

// Current test

const ITestPtr& CurrentTest();

// Access only from test routine
// Thread-safe!

Duration TestTimeLimit();
Duration TestTimeLeft();
// Deterministic
size_t TestHash();

////////////////////////////////////////////////////////////////////////////////

TestList FilterTests(const TestList& tests, ITestFilterPtr filter);
TestList FilterTestSuites(const TestList& tests,
                          std::vector<std::string> suites);

void RunTests(const TestList& tests, const GlobalOptions& options);

////////////////////////////////////////////////////////////////////////////////

}  // namespace wheels::test

#define RUN_ALL_TESTS()                                \
  int main(int argc, const char** argv) {              \
    auto all_tests = wheels::test::ListAllTests();     \
    wheels::test::RunTestsMain(all_tests, argc, argv); \
    return EXIT_SUCCESS;                               \
  }

// Run test suites (from different translation units) in specified order
// Usage: RUN_TEST_SUITES(Unit, Error, Result)

#define RUN_TEST_SUITES(...)                                               \
  int main(int argc, const char** argv) {                                  \
    std::vector<std::string> suites;                                       \
    MAP(_ADD_SUITE_TO_LIST, __VA_ARGS__)                                   \
    auto all_tests = wheels::test::ListAllTests();                         \
    auto suites_tests = wheels::test::FilterTestSuites(all_tests, suites); \
    wheels::test::RunTestsMain(suites_tests, argc, argv);                  \
    return EXIT_SUCCESS;                                                   \
  }

#define _ADD_SUITE_TO_LIST(s) suites.emplace_back(#s);

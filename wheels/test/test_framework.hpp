#pragma once

#include <wheels/test/test.hpp>
#include <wheels/test/registry.hpp>
#include <wheels/test/filter.hpp>
#include <wheels/test/sanitizers.hpp>
#include <wheels/test/main.hpp>

#include <wheels/support/nullptr.hpp>
#include <wheels/support/preprocessor.hpp>
#include <wheels/support/source_location.hpp>
#include <wheels/support/string_builder.hpp>
#include <wheels/support/time.hpp>

#include <iostream>
#include <chrono>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

class AssertionError {
 public:
  AssertionError(const std::string& cond, const SourceLocation& where)
      : cond_(cond), where_(where) {
  }

  template <typename T>
  AssertionError& operator<<(const T& next) {
    description_ << next;
    return *this;
  }

  const SourceLocation& GetLocation() const {
    return where_;
  }

  std::string GetDescription() const {
    return description_.str();
  }

  friend std::ostream& operator<<(std::ostream& out,
                                  const AssertionError& error);

  std::string ToString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
  }

 private:
  std::string cond_;
  SourceLocation where_;
  std::ostringstream description_;
};

std::ostream& operator<<(std::ostream& out, const AssertionError& error);

#define ASSERTION_ERROR(cond) \
  ::wheels::test::AssertionError(TO_STRING(cond), WHEELS_HERE)

////////////////////////////////////////////////////////////////////////////////

void FailTest(const std::string& error_message);
void FailTestByAssert(const AssertionError& assert_error);
void FailTestByException();

////////////////////////////////////////////////////////////////////////////////

#define ASSERT_TRUE(cond)                                    \
  if (!(cond)) {                                             \
    ::wheels::test::FailTestByAssert(ASSERTION_ERROR(cond)); \
  }

#define ASSERT_TRUE_M(cond, message)                                    \
  if (!(cond)) {                                                        \
    ::wheels::test::FailTestByAssert(ASSERTION_ERROR(cond) << message); \
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

class TestTimeLimitWatcher {
 public:
  TestTimeLimitWatcher(wheels::Duration timeout);
  ~TestTimeLimitWatcher();

 private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

////////////////////////////////////////////////////////////////////////////////

inline wheels::Duration TestTimeLimit(wheels::Duration base_time_limit) {
  return base_time_limit * GetSanitizerSlowdown();
}

////////////////////////////////////////////////////////////////////////////////

// Test suite provides separate namespace for bunch of simple test functions

#define TEST_SUITE_WITH_PRIORITY(name, priority) \
  namespace TestSuite##name {                    \
    std::string GetCurrentTestSuiteName() {      \
      return #name;                              \
    }                                            \
                                                 \
    int GetCurrentTestSuitePriority() {          \
      return priority;                           \
    }                                            \
  }                                              \
  namespace TestSuite##name

#define TEST_SUITE(name) TEST_SUITE_WITH_PRIORITY(name, 0)

#define TEST(name, options)                                         \
  void TestRoutine##name();                                         \
  struct Test##name : public ::wheels::test::ITest {                \
    std::string Name() const override {                             \
      return GetCurrentTestSuiteName() + ":" #name;                 \
    }                                                               \
    std::string Describe() const override {                         \
      return #name;                                                 \
    }                                                               \
    std::string Suite() const override {                            \
      return GetCurrentTestSuiteName();                             \
    }                                                               \
    ::wheels::test::TestOptions Options() const override {          \
      return options;                                               \
    }                                                               \
    int Priority() const override {                                 \
      return GetCurrentTestSuitePriority();                         \
    }                                                               \
    void Run() override {                                           \
      ::wheels::test::TestTimeLimitWatcher time_limit_watcher(      \
          ::wheels::test::TestTimeLimit(Options().time_limit));     \
      try {                                                         \
        TestRoutine##name();                                        \
      } catch (...) {                                               \
        ::wheels::test::FailTestByException();                      \
      }                                                             \
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

TestList FilterTests(const TestList& tests, ITestFilterPtr filter);
void RunTests(const TestList& tests);

////////////////////////////////////////////////////////////////////////////////

}  // namespace wheels::test

#define RUN_ALL_TESTS()                     \
  int main(int argc, const char** argv) {   \
    wheels::test::RunTestsMain(argc, argv); \
    return EXIT_SUCCESS;                    \
  }

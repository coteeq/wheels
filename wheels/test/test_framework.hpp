#pragma once

#include <wheels/test/test.hpp>
#include <wheels/test/test_filter.hpp>

#include <wheels/support/nullptr.hpp>
#include <wheels/support/preprocessor.hpp>
#include <wheels/support/source_location.hpp>
#include <wheels/support/string_builder.hpp>
#include <wheels/support/time.hpp>

#include <vector>
#include <chrono>

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

#define ASSERTION_ERROR(cond) AssertionError(TO_STRING(cond), HERE())

////////////////////////////////////////////////////////////////////////////////

void FailTest(const std::string& error_message);
void FailTestByAssert(const AssertionError& assert_error);
void FailTestByException();

////////////////////////////////////////////////////////////////////////////////

#define ASSERT_TRUE(cond)                    \
  if (!(cond)) {                             \
    FailTestByAssert(ASSERTION_ERROR(cond)); \
  }

#define ASSERT_TRUE_M(cond, message)                    \
  if (!(cond)) {                                        \
    FailTestByAssert(ASSERTION_ERROR(cond) << message); \
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

#define FAIL_TEST(error) FailTest(wheels::StringBuilder() << error)

////////////////////////////////////////////////////////////////////////////////

using TestList = std::vector<ITestPtr>;

void RegisterTest(ITestPtr test);
TestList ListAllTests();

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

#define SIMPLE_TEST(name)                                                     \
  void ExecuteTest##name();                                                   \
  struct Test##name : public ITest {                                          \
    std::string Name() const override {                                       \
      return GetCurrentTestSuiteName() + ":" #name;                           \
    }                                                                         \
    std::string Describe() const override {                                   \
      return wheels::StringBuilder() << "'" << #name << "' from test suite '" \
                                     << GetCurrentTestSuiteName() << "'";     \
    }                                                                         \
    int Priority() const override {                                           \
      return GetCurrentTestSuitePriority();                                   \
    }                                                                         \
    void Run() override {                                                     \
      TestTimeLimitWatcher time_limit_watcher(std::chrono::seconds(10));      \
      try {                                                                   \
        ExecuteTest##name();                                                  \
      } catch (...) {                                                         \
        FailTestByException();                                                \
      }                                                                       \
    }                                                                         \
  };                                                                          \
  struct Test##name##Registrar {                                              \
    Test##name##Registrar() {                                                 \
      RegisterTest(std::make_shared<Test##name>());                           \
    }                                                                         \
  };                                                                          \
  static Test##name##Registrar test_##name##_registrar_;                      \
  void ExecuteTest##name()

////////////////////////////////////////////////////////////////////////////////

// Tests with parameters and set of test cases

#define TEST_WITH_PARAMETERS(name)     \
  namespace Test##name {               \
    std::string GetCurrentTestName() { \
      return #name;                    \
    }                                  \
  }                                    \
  namespace Test##name

#define DEFINE_PARAMETER_FLUENT_SETTER(name, type) \
  TestParameters& name(type value) {               \
    name##_ = value;                               \
    return *this;                                  \
  }

#define TEST_PARAMETER(name, type) \
  type name##_;                    \
  DEFINE_PARAMETER_FLUENT_SETTER(name, type)

#define TEST_PARAMETER_DEF(name, type, value) \
  type name##_{value};                        \
  DEFINE_PARAMETER_FLUENT_SETTER(name, type)

#define TEST_TIME_LIMIT_IN_SECS(count)                     \
  TEST_PARAMETER_DEF(time_limit, std::chrono::nanoseconds, \
                     std::chrono::seconds(count))

#define TEST_DEFAULT_TIME_LIMIT() TEST_TIME_LIMIT_IN_SECS(60)

#define REGISTER_TEST_CASES(name, ...)                                    \
  namespace Test##name {                                                  \
    class TestCase : public ITest {                                       \
     public:                                                              \
      TestCase(TestParameters parameters)                                 \
          : parameters_(std::move(parameters)) {                          \
      }                                                                   \
                                                                          \
      std::string Name() const override {                                 \
        return #name;                                                     \
      }                                                                   \
      std::string Describe() const override {                             \
        return StringBuilder()                                            \
               << "'" << #name << "' with parameters " << parameters_;    \
      }                                                                   \
      void Run() override {                                               \
        TestTimeLimitWatcher time_limit_watcher(parameters_.time_limit_); \
        try {                                                             \
          ::Test##name::ExecuteTest(parameters_);                         \
        } catch (...) {                                                   \
          FailTestByException();                                          \
        }                                                                 \
      }                                                                   \
                                                                          \
     private:                                                             \
      TestParameters parameters_;                                         \
    };                                                                    \
                                                                          \
    struct TestCasesRegistrar {                                           \
      TestCasesRegistrar(const std::vector<TestParameters> cases) {       \
        for (auto&& p : cases) {                                          \
          RegisterTest(std::make_shared<TestCase>(p));                    \
        }                                                                 \
      }                                                                   \
    };                                                                    \
    static TestCasesRegistrar registrar(__VA_ARGS__);                     \
  }

////////////////////////////////////////////////////////////////////////////////

TestList FilterTests(const TestList& tests, ITestFilterPtr filter);
void RunTests(const TestList& tests);

////////////////////////////////////////////////////////////////////////////////

#define RUN_ALL_TESTS()                                          \
  int main(int argc, const char** argv) {                        \
    auto filter = CreateTestFilter(argc, argv);                  \
    auto tests = FilterTests(ListAllTests(), std::move(filter)); \
    RunTests(tests);                                             \
    return EXIT_SUCCESS;                                         \
  }

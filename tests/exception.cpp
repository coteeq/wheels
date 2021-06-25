#include <wheels/support/exception.hpp>

#include <wheels/test/test_framework.hpp>

TEST_SUITE(Exceptions) {
  SIMPLE_TEST(CurrentExceptionMessage) {
    try {
      throw std::runtime_error("Test12345");
    } catch (...) {
      ASSERT_EQ(wheels::CurrentExceptionMessage(), "Test12345");
    }
  }
}


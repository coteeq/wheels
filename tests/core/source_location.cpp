#include <wheels/test/test_framework.hpp>

using wheels::SourceLocation;

TEST_SUITE(SourceLocation) {
  SIMPLE_TEST(Here) {
    ASSERT_EQ(wheels::Here().Line(), 7);
    ASSERT_TRUE(wheels::Here().File().ends_with("/source_location.cpp"));
  }

  SIMPLE_TEST(OutOperator) {
    std::cout << "Here = " << wheels::Here() << std::endl;
  }
}

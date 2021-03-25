#include <wheels/support/vector.hpp>

#include <wheels/test/test_framework.hpp>

#include <memory>

TEST_SUITE(ToVector) {
  SIMPLE_TEST(JustWorks) {
    auto v = wheels::ToVector(1, 2, 3);
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v[0], 1);
    ASSERT_EQ(v[1], 2);
    ASSERT_EQ(v[2], 3);
  }

  SIMPLE_TEST(MoveOnly) {
    std::unique_ptr<int> ptr1;
    std::unique_ptr<int> ptr2;

    auto v = wheels::ToVector(std::move(ptr1), std::move(ptr2));
  }
}
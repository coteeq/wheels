#include <wheels/test/test_framework.hpp>

#include <wheels/support/exchange.hpp>

TEST_SUITE(Exchange) {
  SIMPLE_TEST(JustWorks) {
    int joe = 1;
    {
      wheels::RollbackGuard set(joe, 2);
      ASSERT_EQ(joe, 2);
    }
    ASSERT_EQ(joe, 1);
  }

  SIMPLE_TEST(Nested) {
    int joe = 1;
    {
      wheels::RollbackGuard set_1(joe, 2);
      {
        ASSERT_EQ(joe, 2);
        wheels::RollbackGuard set_2(joe, 3);
        ASSERT_EQ(joe, 3);
      }
      ASSERT_EQ(joe, 2);
    }
    ASSERT_EQ(joe, 1);
  }
}

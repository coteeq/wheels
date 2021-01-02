#include <wheels/test/test_framework.hpp>

#include <wheels/support/exchange.hpp>

TEST_SUITE(Exchange) {
  SIMPLE_TEST(JustWorks) {
    int joe = 1;
    {
      auto guard = wheels::ScopedExchange(joe, 2);
      ASSERT_EQ(joe, 2);
    }
    ASSERT_EQ(joe, 1);
  }

  SIMPLE_TEST(Nested) {
    int joe = 1;
    {
      auto outer = wheels::ScopedExchange(joe, 2);
      {
        ASSERT_EQ(joe, 2);
        auto nested = wheels::ScopedExchange(joe, 3);
        ASSERT_EQ(joe, 3);
      }
      ASSERT_EQ(joe, 2);
    }
    ASSERT_EQ(joe, 1);
  }
}

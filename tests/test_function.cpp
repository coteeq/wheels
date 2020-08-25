#include <wheels/support/function.hpp>

#include <wheels/test/test_framework.hpp>

TEST_SUITE(UniqueFunction) {
  SIMPLE_TEST(MoveOnlyLambda) {
    auto data = std::make_unique<std::string>("Hello!");
    wheels::UniqueFunction<void()> f([data = std::move(data)]() {
      ASSERT_EQ(*data, "Hello!");
    });
    auto g = std::move(f);
    g();
  }

  SIMPLE_TEST(CopyConstructible) {
    using F = wheels::UniqueFunction<void()>;
    ASSERT_FALSE(std::is_copy_constructible<F>::value);
  }
}
#include <wheels/support/time.hpp>
#include <wheels/support/sleep.hpp>

#include <wheels/test/test_framework.hpp>

using wheels::Timer;

TEST_SUITE(Time) {
  SIMPLE_TEST(Elapsed) {
    Timer timer;
    wheels::SleepMillis(100);
    ASSERT_TRUE(timer.Elapsed() >= std::chrono::milliseconds(100));
  }

  SIMPLE_TEST(Restart) {
    static const auto kPeriod = std::chrono::milliseconds(100);

    Timer timer;

    wheels::Sleep(kPeriod);
    auto elapsed = timer.Restart();
    ASSERT_TRUE(elapsed >= kPeriod);

    wheels::Sleep(kPeriod);
    elapsed = timer.Elapsed();
    ASSERT_TRUE(elapsed >= kPeriod);
    ASSERT_TRUE(elapsed <= kPeriod * 2);
  }
}

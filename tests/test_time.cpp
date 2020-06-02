#include <wheels/support/time.hpp>
#include <wheels/support/sleep.hpp>

#include <wheels/test/test_framework.hpp>

using wheels::StopWatch;
using namespace std::chrono_literals;

TEST_SUITE(Time) {
  SIMPLE_TEST(Elapsed) {
    StopWatch stop_watch;
    wheels::SleepMillis(100);
    ASSERT_TRUE(stop_watch.Elapsed() >= 100ms);
  }

  SIMPLE_TEST(Restart) {
    static const auto kPeriod = 100ms;

    StopWatch stop_watch;

    wheels::Sleep(kPeriod);
    auto elapsed = stop_watch.Restart();
    ASSERT_TRUE(elapsed >= kPeriod);

    wheels::Sleep(kPeriod);
    elapsed = stop_watch.Elapsed();
    ASSERT_TRUE(elapsed >= kPeriod);
    ASSERT_TRUE(elapsed <= kPeriod * 2);
  }
}

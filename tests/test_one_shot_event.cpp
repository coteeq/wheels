#include <wheels/support/one_shot_event.hpp>

#include <wheels/support/sleep.hpp>
#include <wheels/test/test_framework.hpp>

#include <chrono>
#include <thread>

TEST_SUITE(OneShotEvent) {
  SIMPLE_TEST(SetAndAwait) {
    wheels::OneShotEvent event;

    event.Set();
    event.Await();
  }

  SIMPLE_TEST(RepeatedCalls) {
    wheels::OneShotEvent event;

    event.Set();
    event.Await();

    event.Set();
    event.Await();
  }

  SIMPLE_TEST(BlockingAwait) {
    wheels::OneShotEvent event;

    std::thread delayed_set([&]() {
      wheels::SleepMillis(500);
      event.Set();
    });

    event.Await();

    delayed_set.join();
  }

  SIMPLE_TEST(TimedWait) {
    wheels::OneShotEvent event;

    ASSERT_FALSE(event.TimedWait(std::chrono::seconds(1)));
    event.Set();
    ASSERT_TRUE(event.TimedWait(std::chrono::seconds(1)));
  }
}

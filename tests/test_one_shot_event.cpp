#include <wheels/support/one_shot_event.hpp>

#include <wheels/test/test_framework.hpp>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

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
      std::this_thread::sleep_for(500ms);
      event.Set();
    });

    event.Await();

    delayed_set.join();
  }

  SIMPLE_TEST(TimedWait) {
    wheels::OneShotEvent event;

    ASSERT_FALSE(event.TimedWait(1s));
    event.Set();
    ASSERT_TRUE(event.TimedWait(1s));
  }
}

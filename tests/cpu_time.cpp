#include <wheels/support/cpu_time.hpp>

#include <wheels/test/test_framework.hpp>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

using wheels::ProcessCPUTimer;
using wheels::ThreadCPUTimer;

TEST_SUITE(CPUTime) {
  double ToSeconds(std::chrono::nanoseconds elapsed) {
    return (double)elapsed.count() / 1'000'000'000;
  }

  SIMPLE_TEST(Work) {
    ThreadCPUTimer timer;
    volatile size_t count = 0;
    for (size_t i = 0; i < 100000000; ++i) {
      ++count;
    }
    ASSERT_TRUE(timer.Elapsed() > 100ms);
  }

  SIMPLE_TEST(Sleep) {
    ThreadCPUTimer timer;
    std::this_thread::sleep_for(100ms);
    ASSERT_TRUE(timer.Elapsed() < 10ms);
  }

  SIMPLE_TEST(PerThread) {
    std::atomic<bool> stop_requested{false};

    std::thread busy([&]() {
      ThreadCPUTimer timer;
      volatile size_t count = 0;
      while (!stop_requested.load()) {
        ++count;
      }
      auto elapsed = timer.Elapsed();
      std::cout << "Busy thread running time: " << ToSeconds(elapsed) << std::endl;
      ASSERT_TRUE(elapsed > 700ms);
    });

    ThreadCPUTimer timer;

    std::this_thread::sleep_for(1s);

    auto elapsed = timer.Elapsed();
    std::cout << "Sleeper running time: " << ToSeconds(elapsed) << std::endl;
    ASSERT_TRUE(elapsed < 100ms);

    stop_requested.store(true);
    busy.join();
  }
}

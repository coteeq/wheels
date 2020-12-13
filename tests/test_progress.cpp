#include <wheels/support/progress.hpp>

#include <wheels/test/test_framework.hpp>

#include <thread>

TEST_SUITE(ProgressBar) {
  SIMPLE_TEST(Works) {
    wheels::ProgressBar progress("Works");

    static const size_t kSteps = 10'000'000;

    progress.Start(kSteps);
    for (size_t i = 0; i < kSteps; ++i) {
      progress.MakeProgress();
      std::this_thread::yield();
    }
  }
}
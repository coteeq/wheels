#include <wheels/support/sleep.hpp>

#include <wheels/support/random.hpp>

#include <chrono>
#include <thread>

namespace wheels {

void Sleep(Duration delay) {
  std::this_thread::sleep_for(delay);
}

void SleepSeconds(size_t secs) {
  Sleep(std::chrono::seconds(secs));
}

void SleepMillis(size_t millis) {
  Sleep(std::chrono::milliseconds(millis));
}

void SleepRandomMillis(size_t lo, size_t hi) {
  SleepMillis(RandomUInteger(lo, hi));
}

}  // namespace wheels

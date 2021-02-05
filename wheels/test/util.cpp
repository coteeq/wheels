#include <wheels/test/util.hpp>

#include <chrono>

using namespace std::chrono_literals;

namespace wheels::test {

bool KeepRunning() {
  return TestTimeLeft() > 100ms;
}

}  // namespace wheels::test

#include <wheels/test/util.hpp>

#include <chrono>

using namespace std::chrono_literals;

namespace wheels::test {

bool KeepRunning() {
  return TestTimeLeft() > 250ms;
}

}  // namespace wheels::test

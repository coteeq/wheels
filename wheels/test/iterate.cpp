#include <wheels/test/iterate.hpp>

#include <wheels/test/helpers.hpp>

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

namespace wheels::test {

class Progress {
 public:
  void IterCompleted() {
    ++iterations_;
  }

  size_t Iterations() const {
    return iterations_;
  }

 private:
  size_t iterations_{0};
};

// TODO: measure iteration time limit
static const auto kSafeTLMargin = 100ms;

void Iterate(std::function<void()> test_routine) {
  std::cout << "Iterated test, time budget: "
    << FormatSeconds(TestTimeLimit(), 2) << " seconds" << std::endl;

  Progress progress;

  while (TestTimeLeft() > kSafeTLMargin) {
    test_routine();
    progress.IterCompleted();
  }

  std::cout << "Iterations made: " << progress.Iterations() << std::endl;
}

}  // namespace wheels::test

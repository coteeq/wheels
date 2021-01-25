#include <wheels/test/iterate.hpp>

#include <wheels/test/helpers.hpp>

#include <wheels/support/hash_combine.hpp>

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

  size_t iter = 0;
  Progress progress;

  size_t test_iter_hash = TestHash();

  while (TestTimeLeft() > kSafeTLMargin) {
    ++iter;
    SetContext("test_iteration_hash", test_iter_hash);
    SetContext("test_iteration", iter);
    test_routine();
    progress.IterCompleted();
    HashCombine(test_iter_hash, iter);
  }

  std::cout << "Iterations made: " << progress.Iterations() << std::endl;
}

size_t TestIteration() {
  return GetContext<size_t>("test_iteration");
}

size_t TestIterationHash() {
  return GetContext<size_t>("test_iteration_hash");
}

}  // namespace wheels::test

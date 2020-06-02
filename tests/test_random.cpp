#include <wheels/support/random.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>
#include <vector>
#include <unordered_set>

TEST_SUITE(Random) {
  void TossUntil(bool result) {
    while (wheels::TossFairCoin() != result) {
      // continue
    }
  }

  SIMPLE_TEST(TestFairCoin) {
    for (size_t i = 0; i < 100; ++i) {
      TossUntil(true);
      TossUntil(false);
    }
  }

  SIMPLE_TEST(RandomNumber) {
    size_t value = wheels::RandomUInteger();
    ASSERT_TRUE(value >= 0 && value <= (size_t)-1);
  }

  SIMPLE_TEST(RandomNumberInRange) {
    static const size_t kIterations = 1000;
    static const size_t kFrom = 17;
    static const size_t kTo = 111;

    for (size_t i = 0; i < kIterations; ++i) {
      size_t x = wheels::RandomUInteger(kFrom, kTo);
      ASSERT_TRUE(x >= kFrom);
      ASSERT_TRUE(x <= kTo);
    }
  }

  std::vector<int> GenerateRandomPermutation(size_t n) {
    std::vector<int> pi;
    pi.reserve(n);
    for (size_t i = 0; i < n; ++i) {
      pi.push_back(i);
    }
    wheels::RandomShuffleInplace(pi);
    return pi;
  }

  struct PermutationHash {
    size_t operator()(const std::vector<int>& pi) const {
      std::hash<int> hasher;
      size_t result = 0;
      for (int i : pi) {
          result ^= hasher(i) + 0x9e3779b9 + (result << 6) + (result >> 2);
      }
      return result;
    }
  };

  size_t Factorial(size_t n) {
    size_t result = 1;
    for (size_t i = 2; i <= n; ++i) {
      result *= i;
    }
    return result;
  }

  SIMPLE_TEST(TestRandomShuffleInplace) {
    std::unordered_set<std::vector<int>, PermutationHash> permutations;

    const size_t kSize = 7;

    size_t all_permutations_count = Factorial(kSize);

    size_t iteration_count = 0;

    // Await all kSize! permutations
    do {
      ++iteration_count;
      auto pi = GenerateRandomPermutation(kSize);
      ASSERT_EQ(pi.size(), kSize);
      permutations.insert(pi);
    } while (permutations.size() != all_permutations_count);

    std::cout << all_permutations_count << " permutations generated in " << iteration_count << " iterations" << std::endl;
  }
}

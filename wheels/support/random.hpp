#pragma once

#include <wheels/support/intrusive_list.hpp>

#include <cstddef>
#include <iterator>
#include <list>
#include <stdexcept>
#include <vector>

namespace wheels {

// All functions are thread-safe

// Returns random unsigned integer in range [0, max]
size_t RandomUInteger(size_t max = -1);

// Returns random unsigned integer in range [lo, hi]
size_t RandomUInteger(size_t lo, size_t hi);

// true - heads, false - tails
bool TossFairCoin();

// Fisher-Yates shuffle algorithm
template <typename T>
void RandomShuffleInplace(std::vector<T>& items) {
  if (items.empty()) {
    return;  // nothing to do
  }

  for (size_t i = items.size() - 1; i > 0; --i) {
    size_t j = RandomUInteger(i);
    if (i != j) {
      std::swap(items[i], items[j]);
    }
  }
}

template <typename T>
T PickRandom(const std::vector<T>& items) {
  if (items.empty()) {
    throw std::invalid_argument("items is empty");
  }
  return items[wheels::RandomUInteger(items.size() - 1)];
}

template <typename T>
T* UnlinkRandomItem(IntrusiveList<T>& items) {
  auto iterator = items.begin();
  std::advance(iterator, RandomUInteger(items.Size() - 1));
  auto* item = iterator.Item();
  item->Unlink();
  return item;
}

template <typename T>
std::vector<T*> ShuffleToVector(IntrusiveList<T>& items) {
  std::vector<T*> shuffled;
  while (!items.IsEmpty()) {
    shuffled.push_back(items.PopFront());
  }
  RandomShuffleInplace(shuffled);
  return shuffled;
}

}  // namespace wheels

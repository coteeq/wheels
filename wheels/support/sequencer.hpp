#pragma once

#include <atomic>

namespace wheels {

// Produces increasing numbers in thread-safe way
// Typically used for id generation

class Sequencer {
 public:
  size_t Next() {
    return next_.fetch_add(1);
  }

 private:
  std::atomic<size_t> next_{0};
};

}  // namespace wheels

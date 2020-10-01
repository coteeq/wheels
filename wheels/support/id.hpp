#pragma once

#include <cstddef>
#include <atomic>

namespace wheels::support {

// Monotonic integer ids

//////////////////////////////////////////////////////////////////////

using Id = std::size_t;

static const Id kInvalidId = 0;

//////////////////////////////////////////////////////////////////////

class IdGenerator {
 public:
  Id NextId() {
    return next_.fetch_add(1) + 1;
  }

 private:
  std::atomic<Id> next_{0};
};

}  // namespace wheels::support

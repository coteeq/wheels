#pragma once

#include <utility>

namespace wheels {

/* Usage:
 * {
 *   // Save old value and set `value`
 *   RollbackGuard set(var, value);
 *   ...
 * } // Rollback saved old value
 */

// TODO: Looking for a better name...
template <typename T>
class RollbackGuard {
 public:
  RollbackGuard(T& obj, T value)
    : obj_(obj), old_value_(std::exchange(obj, std::move(value))) {
  }

  ~RollbackGuard() {
    std::swap(obj_, old_value_);
  }

 private:
  T& obj_;
  T old_value_;
};

}  // namespace wheels

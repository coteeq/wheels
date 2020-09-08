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
  RollbackGuard(T& obj, T old_value)
    : obj_(obj), old_value_(old_value) {
  }

  ~RollbackGuard() {
    std::swap(obj_, old_value_);
  }

 private:
  T& obj_;
  T old_value_;
};

template <typename T>
RollbackGuard<T> ScopedExchange(T& obj, T value) {
  return RollbackGuard<T>(obj, std::exchange(obj, value));
}

}  // namespace wheels

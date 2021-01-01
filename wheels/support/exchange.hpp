#pragma once

#include <utility>

namespace wheels {

namespace detail {

template <typename T>
class RollbackGuard {
 public:
  RollbackGuard(T& obj, T&& old_value)
      : obj_(obj), old_value_(std::move(old_value)) {
  }

  ~RollbackGuard() {
    std::swap(obj_, old_value_);
  }

 private:
  T& obj_;
  T old_value_;
};

}  // namespace detail

template <typename T>
detail::RollbackGuard<T> ScopedExchange(T& obj, T value) {
  return detail::RollbackGuard<T>(obj, std::exchange(obj, std::move(value)));
}

}  // namespace wheels

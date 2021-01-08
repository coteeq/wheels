#pragma once

#include <cstddef>
#include <cstdint>

namespace wheels {

// View on a contiguous chunk of writeable memory
// The memory is owned by some other object and that object must maintain the
// memory as long as the span references it.

struct MemSpan {
 public:
  MemSpan(char* start, size_t size) : start_(start), size_(size) {
  }

  MemSpan() : MemSpan(nullptr, 0) {
  }

  size_t Size() const noexcept {
    return size_;
  }

  char* Begin() const noexcept {
    return start_;
  }

  char* End() const noexcept {
    return start_ + size_;
  }

  char* Data() const noexcept {
    return Begin();
  }

  char* Back() const noexcept {
    return End() - 1;
  }

 private:
  char* start_;
  size_t size_;
};

}  // namespace wheels

#pragma once

#include <wheels/memory/view.hpp>

#include <vector>

namespace wheels {

struct GrowingBuffer {
 public:
  GrowingBuffer() = default;

  void Reserve(size_t capacity) {
    buf_.reserve(capacity);
  }

  void Clear() {
    buf_.resize(0);
  }

  void Append(std::string_view data) {
    ReserveForAppend(data.size());
    buf_.insert(buf_.end(), data.begin(), data.end());
  }

  void Append(ConstMemView data) {
    buf_.insert(buf_.end(), data.Begin(), data.End());
  }

  ConstMemView View() const {
    return {buf_.data(), buf_.size()};
  }

  void ShrinkToFit() {
    buf_.shrink_to_fit();
  }

 private:
  void ReserveForAppend(size_t size) {
    buf_.reserve(buf_.size() + size);
  }

 private:
  std::vector<char> buf_;
};

}  // namespace wheels

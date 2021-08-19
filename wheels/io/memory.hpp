#pragma once

#include <wheels/io/reader.hpp>

#include <algorithm>

namespace wheels::io {

class MemoryReader : public IReader {
 public:
  MemoryReader(ConstMemView source) : source_(source) {
  }

  MemoryReader() : MemoryReader({nullptr, 0}) {
  }

  size_t ReadSome(MutableMemView buffer) override {
    return Read(buffer);
  }

  size_t Read(MutableMemView buffer) {
    size_t bytes_to_read = std::min(buffer.Size(), source_.Size());
    if (bytes_to_read > 0) {
      memcpy(buffer.Data(), source_.Data(), bytes_to_read);
      source_ += bytes_to_read;
    }
    return bytes_to_read;
  }

  void Reset(ConstMemView source) {
    source_ = source;
  }

  bool Exhausted() const noexcept {
    return source_.IsEmpty();
  }

 private:
  ConstMemView source_;
};

}  // namespace wheels::io
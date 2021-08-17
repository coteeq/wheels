#pragma once

#include <wheels/io/reader.hpp>

#include <algorithm>

namespace wheels::io {

class LimitReader : public IReader {
 public:
  LimitReader(IReader* from, size_t limit) : from_(from), left_(limit) {
  }

  size_t ReadSome(MutableMemView buffer) {
    size_t bytes_to_read = std::min(left_, buffer.Size());
    if (bytes_to_read == 0) {
      return 0;
    }
    size_t bytes_read = from_->ReadSome({buffer.Data(), bytes_to_read});
    left_ -= bytes_read;
    return bytes_read;
  }

 private:
  IReader* from_;
  size_t left_;
};

}  // namespace wheels::io

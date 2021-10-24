#pragma once

#include <wheels/io/reader.hpp>
#include <wheels/result/make.hpp>

#include <algorithm>

namespace wheels::io {

class LimitReader : public IReader {
 public:
  LimitReader(IReader* from, size_t limit) : from_(from), left_(limit) {
  }

  Result<size_t> ReadSome(MutableMemView buffer) {
    size_t bytes_to_read = std::min(left_, buffer.Size());
    if (bytes_to_read == 0) {
      return make_result::Ok<size_t>(0);
    }
    auto bytes_read = from_->ReadSome({buffer.Data(), bytes_to_read});
    if (!bytes_read.IsOk()) {
      return make_result::PropagateError(bytes_read);
    }
    left_ -= *bytes_read;
    return make_result::Ok(*bytes_read);
  }

 private:
  IReader* from_;
  size_t left_;
};

}  // namespace wheels::io

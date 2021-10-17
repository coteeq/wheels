#pragma once

#include <wheels/io/reader.hpp>
#include <wheels/io/memory.hpp>

#include <wheels/memory/view_of.hpp>

namespace wheels::io {

//////////////////////////////////////////////////////////////////////

class BufferedReader : public IReader {
  static const size_t kDefaultBufSize = 1024;

 public:
  BufferedReader(IReader* from, size_t buf_size = kDefaultBufSize)
      : from_(from), buf_(buf_size) {
    buf_reader_.Reset();
  }

  Result<size_t> ReadSome(MutableMemView dest) override {
    if (!buf_reader_.Exhausted()) {
      return buf_reader_.Read(dest);
    }

    // Refill buffer
    auto refilled = ReFillBuffer();
    if (!refilled.IsOk()) {
      return make_result::PropagateError(refilled);
    }

    if (!buf_reader_.Exhausted()) {
      return buf_reader_.Read(dest);
    } else {
      return make_result::Ok<size_t>(0);
    }
  }

 private:
  Status ReFillBuffer() {
    auto bytes_read = from_->ReadSome(MutViewOf(buf_));
    if (!bytes_read.IsOk()) {
      return make_result::PropagateError(bytes_read);
    }
    if (*bytes_read > 0) {
      buf_reader_.Reset({buf_.data(), *bytes_read});
    } else {
      buf_reader_.Reset();
    }
    return make_result::Ok();
  }

 private:
  IReader* from_;
  std::vector<char> buf_;
  MemoryReader buf_reader_;
};

//////////////////////////////////////////////////////////////////////

// TODO: BufferedWriter

}  // namespace wheels::io

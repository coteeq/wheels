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
    buf_reader_.Reset(ConstMemView::Empty());
  }

  size_t ReadSome(MutableMemView dest) override {
    if (!buf_reader_.Exhausted() || ReFillBuffer()) {
      return buf_reader_.Read(dest);
    } else {
      return 0;
    }
  }

 private:
  bool ReFillBuffer() {
    size_t bytes_read = from_->ReadSome(MutViewOf(buf_));
    if (bytes_read > 0) {
      buf_reader_.Reset({buf_.data(), bytes_read});
    }
    return bytes_read > 0;
  }

 private:
  IReader* from_;
  std::vector<char> buf_;
  MemoryReader buf_reader_;
};

//////////////////////////////////////////////////////////////////////

// TODO: BufferedWriter

}  // namespace wheels::io

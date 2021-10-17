#pragma once

#include <wheels/memory/view.hpp>
#include <wheels/result/result.hpp>

namespace wheels::io {

//////////////////////////////////////////////////////////////////////

struct IReader {
  virtual ~IReader() = default;

  // 0 – end of stream
  virtual Result<size_t> ReadSome(MutableMemView buffer) = 0;
};

}  // namespace wheels::io

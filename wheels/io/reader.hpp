#pragma once

#include <wheels/support/mem_view.hpp>

namespace wheels::io {

//////////////////////////////////////////////////////////////////////

struct IReader {
  virtual ~IReader() = default;

  virtual size_t ReadSome(MutableMemView buffer) = 0;
};

}  // namespace wheels::io
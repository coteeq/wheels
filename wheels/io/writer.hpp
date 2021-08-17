#pragma once

#include <wheels/support/memspan.hpp>

namespace wheels::io {

struct IWriter {
  virtual ~IWriter() = default;

  virtual void Write(wheels::ConstMemView data) = 0;
};

}  // namespace wheels::io

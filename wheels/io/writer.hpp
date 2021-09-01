#pragma once

#include <wheels/memory/view.hpp>

namespace wheels::io {

struct IWriter {
  virtual ~IWriter() = default;

  virtual void Write(ConstMemView data) = 0;
  virtual void Flush() = 0;
};

}  // namespace wheels::io

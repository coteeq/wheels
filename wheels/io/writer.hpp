#pragma once

#include <wheels/memory/view.hpp>
#include <wheels/result/result.hpp>

namespace wheels::io {

struct IWriter {
  virtual ~IWriter() = default;

  virtual Status Write(ConstMemView data) = 0;
  virtual Status Flush() = 0;
};

}  // namespace wheels::io

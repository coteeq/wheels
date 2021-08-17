#pragma once

#include <wheels/io/reader.hpp>
#include <wheels/io/writer.hpp>

#include <wheels/support/view_util.hpp>

#include <string>

namespace wheels::io {

class StringWriter : public IWriter {
 public:
  StringWriter(std::string& str) : dest_(str) {
  }

  void Write(ConstMemView data) override {
    dest_.append(data.Begin(), data.Size());
  }

 private:
  std::string& dest_;
};

}  // namespace wheels::io

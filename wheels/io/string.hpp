#pragma once

#include <wheels/io/reader.hpp>
#include <wheels/io/writer.hpp>

#include <wheels/memory/view_of.hpp>

#include <string>

namespace wheels::io {

class StringWriter : public IWriter {
 public:
  StringWriter(std::string& str) : dest_(str) {
  }

  void Write(ConstMemView data) override {
    dest_.append(data.Begin(), data.Size());
  }

  void Flush() override {
    // Nothing to do
  }

 private:
  std::string& dest_;
};

}  // namespace wheels::io

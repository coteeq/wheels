#pragma once

#include <wheels/io/reader.hpp>
#include <wheels/io/writer.hpp>

#include <wheels/memory/view_of.hpp>
#include <wheels/result/make.hpp>

#include <string>

namespace wheels::io {

class StringWriter : public IWriter {
 public:
  explicit StringWriter(std::string& str) : dest_(str) {
  }

  Status Write(ConstMemView data) override {
    dest_.append(data.Begin(), data.Size());
    return make_result::Ok();
  }

  Status Flush() override {
    return make_result::Ok();
  }

 private:
  std::string& dest_;
};

}  // namespace wheels::io

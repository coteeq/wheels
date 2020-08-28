#pragma once

#include <wheels/test/test_options.hpp>

#include <memory>
#include <string>

namespace wheels::test {

class ITest {
 public:
  virtual ~ITest() = default;

  virtual void Run() = 0;
  virtual std::string Name() const = 0;
  virtual std::string Suite() const = 0;
  virtual std::string Describe() const = 0;

  virtual TestOptions Options() const {
    return {};  // defaults
  }

  virtual int Priority() const {
    return 0;  // default
  }
};

using ITestPtr = std::shared_ptr<ITest>;

}  // namespace wheels::test

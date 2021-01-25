#pragma once

#include <wheels/test/test_options.hpp>

#include <memory>
#include <string>
#include <vector>

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
};

using ITestPtr = std::shared_ptr<ITest>;

using TestList = std::vector<ITestPtr>;

}  // namespace wheels::test

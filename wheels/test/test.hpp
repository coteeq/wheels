#pragma once

#include <memory>
#include <string>

////////////////////////////////////////////////////////////////////////////////

class ITest {
 public:
  virtual ~ITest() = default;

  virtual void Run() = 0;
  virtual std::string Name() const = 0;
  virtual std::string Describe() const = 0;

  virtual int Priority() const {
    return 0;  // default
  }
};

using ITestPtr = std::shared_ptr<ITest>;

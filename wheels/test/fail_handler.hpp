#pragma once

#include <wheels/test/test.hpp>

#include <memory>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

class ITestFailHandler {
 public:
  virtual ~ITestFailHandler() = default;

  virtual void Fail(ITestPtr test, const std::string& error) = 0;
};

using ITestFailHandlerPtr = std::shared_ptr<ITestFailHandler>;

////////////////////////////////////////////////////////////////////////////////

void InstallTestFailHandler(ITestFailHandlerPtr handler);

ITestFailHandlerPtr GetTestFailHandler();

////////////////////////////////////////////////////////////////////////////////

class FailHandlerSwitcher {
 public:
  FailHandlerSwitcher(ITestFailHandlerPtr handler)
      : current_(GetTestFailHandler()) {
    InstallTestFailHandler(handler);
  }

  ~FailHandlerSwitcher() {
    InstallTestFailHandler(current_);
  }

 private:
  ITestFailHandlerPtr current_;
};

}  // namespace wheels::test

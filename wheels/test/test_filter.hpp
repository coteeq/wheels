#pragma once

#include <wheels/test/test.hpp>

namespace wheels::test {

class ITestFilter {
 public:
  virtual ~ITestFilter() = default;

  virtual bool Accept(const ITestPtr& test) const = 0;
};

using ITestFilterPtr = std::shared_ptr<ITestFilter>;

ITestFilterPtr CreateTestFilter(int argc, const char** argv);

}  // namespace wheels::test

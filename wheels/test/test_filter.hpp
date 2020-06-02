#pragma once

#include <wheels/test/test.hpp>

class ITestFilter {
 public:
  virtual ~ITestFilter() = default;

  virtual bool Accept(const ITestPtr& test) const = 0;
};

using ITestFilterPtr = std::shared_ptr<ITestFilter>;

ITestFilterPtr CreateTestFilter(int argc, const char** argv);

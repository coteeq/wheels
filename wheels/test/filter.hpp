#pragma once

#include <wheels/test/test.hpp>

#include <wheels/support/args.hpp>

#include <memory>

namespace wheels::test {

class ITestFilter {
 public:
  virtual ~ITestFilter() = default;

  virtual bool Accept(const ITestPtr& test) const = 0;
};

using ITestFilterPtr = std::shared_ptr<ITestFilter>;

ITestFilterPtr CreateTestFilter(const NamedArgs& args);

}  // namespace wheels::test

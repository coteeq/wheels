#pragma once

#include <wheels/test/test_options.hpp>

#include <wheels/support/time.hpp>

#include <memory>

namespace wheels::test {

class TestTimeLimitWatcher {
 public:
  TestTimeLimitWatcher(wheels::Duration timeout);
  ~TestTimeLimitWatcher();

 private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

wheels::Duration TestTimeLimit(TestOptions options);

}  // namespace wheels::test

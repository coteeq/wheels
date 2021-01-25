#include <wheels/test/registry.hpp>

#include <wheels/support/singleton.hpp>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

class TestRepository {
 public:
  void Register(ITestPtr test) {
    all_tests_.push_back(std::move(test));
  }

  TestList ListAll() const {
    return all_tests_;
  }

 private:
  TestList all_tests_;
};

void RegisterTest(ITestPtr test) {
  Singleton<TestRepository>()->Register(std::move(test));
}

TestList ListAllTests() {
  return Singleton<TestRepository>()->ListAll();
}

}  // namespace wheels::test

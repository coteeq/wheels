#include <wheels/test/registry.hpp>

#include <wheels/support/singleton.hpp>

#include <map>

namespace wheels::test {

////////////////////////////////////////////////////////////////////////////////

class TestRepository {
 public:
  void Register(ITestPtr test) {
    int priority = test->Priority();
    tests_[priority].push_back(std::move(test));
  }

  TestList ListAll() const {
    TestList all;
    for (const auto& [priority, tests] : tests_) {
      for (const auto& t : tests) {
        all.push_back(t);
      }
    }
    return all;
  }

 private:
  // priority -> test list
  std::map<int, TestList> tests_;
};

void RegisterTest(ITestPtr test) {
  Singleton<TestRepository>()->Register(std::move(test));
}

TestList ListAllTests() {
  return Singleton<TestRepository>()->ListAll();
}

}  // namespace wheels::test

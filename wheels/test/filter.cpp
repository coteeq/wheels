#include <wheels/test/filter.hpp>

#include <cstdlib>
#include <regex>
#include <string>

namespace wheels::test {

class RegexTestFilter : public ITestFilter {
 public:
  RegexTestFilter(std::string pattern) : regex_(std::move(pattern)) {
  }

  bool Accept(const ITestPtr& test) const override {
    return regex_search(test->Describe(), regex_);
  }

 private:
  std::regex regex_;
};

ITestFilterPtr CreateTestFilter(int, const char**) {
  const char* value = getenv("WHEELS_TEST_FILTER");
  return std::make_shared<RegexTestFilter>(value ? value : "");
}

}  // namespace wheels::test

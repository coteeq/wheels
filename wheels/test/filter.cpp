#include <wheels/test/filter.hpp>

#include <wheels/cmdline/argparse.hpp>

#include <regex>

namespace wheels::test {

class RegexTestFilter : public ITestFilter {
 public:
  RegexTestFilter(const std::string& suite, const std::string test)
      : suite_(std::move(suite)), test_(std::move(test)) {
  }

  bool Accept(const ITestPtr& test) const override {
    return regex_search(test->Suite(), suite_) &&
           regex_search(test->Name(), test_);
  }

 private:
  std::regex suite_;
  std::regex test_;
};

ITestFilterPtr CreateTestFilter(const ParsedArgs& args) {
  const auto suite = args.Get("suite");
  const auto test = args.Get("test");

  return std::make_shared<RegexTestFilter>(suite, test);
}

}  // namespace wheels::test

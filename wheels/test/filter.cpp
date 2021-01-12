#include <wheels/test/filter.hpp>

#include <wheels/support/args.hpp>

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

static std::string GetArgumentOr(const NamedArgs& args, const std::string& name,
                                 const std::string& default_value) {
  auto it = args.find(name);
  if (it != args.end()) {
    return it->second;
  } else {
    return default_value;
  }
}

ITestFilterPtr CreateTestFilter(const NamedArgs& args) {
  const auto suite = GetArgumentOr(args, "--suite", "");
  const auto test = GetArgumentOr(args, "--test", "");

  return std::make_shared<RegexTestFilter>(suite, test);
}

}  // namespace wheels::test

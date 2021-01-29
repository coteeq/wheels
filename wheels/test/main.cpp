#include <wheels/test/main.hpp>

#include <wheels/test/test_framework.hpp>

#include <wheels/support/args.hpp>
#include <wheels/support/string_utils.hpp>

#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace wheels::test {

static Options MakeOptions(const ParsedArgs& args) {
  Options options;
  options.forks = !args.HasFlag("no-forks");
  options.disable_time_limits = args.HasFlag("disable-time-limits");
  return options;
}

void RunTestsMain(const TestList& tests, int argc, const char** argv) {
  ArgumentsParser parser{"Wheels test framework"};
  // Filter
  parser.Add(Argument{"suite"}.ValueDescr("REGEXP").Default(".*"));
  parser.Add(Argument{"test"}.ValueDescr("REGEXP").Default(".*"));
  parser.AddFlag("no-forks");
  parser.AddFlag("disable-time-limits");

  const auto args = parser.Parse(argc, argv);

  auto filter = CreateTestFilter(args);
  auto selected_tests = FilterTests(tests, std::move(filter));

  auto options = MakeOptions(args);

  RunTests(selected_tests, options);
}

}  // namespace wheels::test

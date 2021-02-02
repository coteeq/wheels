#include <wheels/test/main.hpp>

#include <wheels/test/test_framework.hpp>

#include <wheels/support/argparse.hpp>
#include <wheels/support/string_utils.hpp>

#include <map>

namespace wheels::test {

static Options MakeOptions(const ParsedArgs& args) {
  Options options;
  options.forks = !args.HasFlag("disable-forks");
  options.disable_time_limits = args.HasFlag("disable-time-limits");
  return options;
}

void RunTestsMain(const TestList& tests, int argc, const char** argv) {
  ArgumentParser parser{"Wheels test framework"};
  parser.AddHelpFlag();
  // Filter
  parser.Add(Argument{"suite"}.ValueDescr("REGEXP").WithDefault(".*"));
  parser.Add(Argument{"test"}.ValueDescr("REGEXP").WithDefault(".*"));
  parser.AddFlag("disable-forks");
  parser.AddFlag("disable-time-limits");

  const auto args = parser.Parse(argc, argv);

  auto filter = CreateTestFilter(args);
  auto selected_tests = FilterTests(tests, std::move(filter));

  auto options = MakeOptions(args);

  RunTests(selected_tests, options);
}

}  // namespace wheels::test

#include <wheels/test/main.hpp>

#include <wheels/test/test_framework.hpp>

#include <wheels/support/argparse.hpp>
#include <wheels/support/string_utils.hpp>

#include <map>

namespace wheels::test {

static GlobalOptions MakeOptions(const ParsedArgs& args) {
  GlobalOptions options;
  options.forks = !args.HasFlag("disable-forks");
  options.disable_time_limits = args.HasFlag("disable-time-limits");
  return options;
}

static void CLI(ArgumentParser& parser) {
  parser.AddHelpFlag();
  // Filter
  parser.Add("suite").ValueDescr("REGEXP").WithDefault(".*").Help(
      "Test suite name filter");
  parser.Add("test").ValueDescr("REGEXP").WithDefault(".*").Help(
      "Test name filter");
  parser.Add("disable-forks")
      .Flag()
      .Help("Do not execute tests in subprocesses");
  parser.Add("disable-time-limits").Flag();
}

void RunTestsMain(const TestList& tests, int argc, const char** argv) {
  ArgumentParser parser{"Wheels test runner"};
  CLI(parser);

  const auto args = parser.Parse(argc, argv);

  auto filter = CreateTestFilter(args);
  auto selected_tests = FilterTests(tests, std::move(filter));

  auto options = MakeOptions(args);

  RunTests(selected_tests, options);
}

}  // namespace wheels::test

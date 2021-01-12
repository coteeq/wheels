#include <wheels/test/main.hpp>

#include <wheels/test/test_framework.hpp>

#include <wheels/support/args.hpp>

#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace wheels::test {

void RunTestsMain(int argc, const char** argv) {
  const auto args = AsNamedArgs(argc, argv);

  if (!args.empty()) {
    std::cout << "Command line arguments: " << std::endl;
    for (const auto& [k, v] : args) {
      std::cout << "'" << k << "' -> '" << v << "'" << std::endl;
    }
  }

  auto filter = CreateTestFilter(args);
  auto tests = FilterTests(ListAllTests(), std::move(filter));

  RunTests(tests);
}

}  // namespace wheels::test

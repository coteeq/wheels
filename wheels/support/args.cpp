#include <wheels/support/args.hpp>

#include <wheels/support/string_utils.hpp>
#include <wheels/support/assert.hpp>

namespace wheels {

NamedArgs AsNamedArgs(int argc, const char** argv) {
  NamedArgs args;
  for (int i = 1; i + 1 < argc; i += 2) {
    std::string name(argv[i]);
    WHEELS_VERIFY(StartsWith(name, "--"),
                  "Argument name expected: --{name}, found '" << name << "'");
    args.emplace(name, std::string(argv[i + 1]));
  }
  return args;
}

}  // namespace wheels

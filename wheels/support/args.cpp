#include <wheels/support/args.hpp>

#include <wheels/support/string_utils.hpp>
#include <wheels/support/assert.hpp>

namespace wheels {

NamedArgs ArgumentsParser::Parse(const int argc, const char** argv) {
  NamedArgs args;

  for (int i = 1; i < argc; i += 2) {
    std::string name(argv[i]);

    WHEELS_VERIFY(StartsWith(name, "--"),
                  "Argument name expected: --{name}, found " << Quoted(name));

    if (arg_names_.find(name) == arg_names_.end()) {
      WHEELS_PANIC("Unexpected command line argument: " << Quoted(name));
    }

    if (i + 1 >= argc) {
      WHEELS_PANIC("Value for command line argument " << Quoted(name) << " not set");
    }

    args.emplace(name, std::string(argv[i + 1]));
  }

  return args;
}

}  // namespace wheels

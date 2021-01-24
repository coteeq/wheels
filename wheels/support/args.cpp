#include <wheels/support/args.hpp>

#include <wheels/support/string_utils.hpp>
#include <wheels/support/assert.hpp>

namespace wheels {

std::string ArgumentsParser::MakeLongArgument(std::string name) {
  return StringBuilder() << "--" << name;
}

std::string ArgumentsParser::GetLongArgumentName(const std::string argument) {
  return argument.substr(2, argument.length());
}

NamedArgs ArgumentsParser::Parse(const int argc, const char** argv) {
  NamedArgs args;

  for (int i = 1; i < argc; i += 2) {
    std::string argument(argv[i]);

    WHEELS_VERIFY(StartsWith(argument, "--"),
                  "Argument expected: --{name}, found " << Quoted(argument));

    if (arg_long_names_.find(argument) == arg_long_names_.end()) {
      WHEELS_PANIC("Unexpected command line argument: " << Quoted(argument));
    }

    if (i + 1 >= argc) {
      WHEELS_PANIC("Value for command line argument " << Quoted(argument)
                                                      << " not set");
    }

    args.emplace(GetLongArgumentName(argument), std::string(argv[i + 1]));
  }

  return args;
}

}  // namespace wheels

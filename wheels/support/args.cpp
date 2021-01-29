#include <wheels/support/args.hpp>

#include <wheels/support/string_utils.hpp>
#include <wheels/support/assert.hpp>

namespace wheels {

std::string ArgumentsParser::WithoutDashes(const std::string argument) {
  return argument.substr(2, argument.length());
}

#define FAIL_PARSE(error) Fail(StringBuilder() << name_ << ": " error)

ParsedArgs ArgumentsParser::Parse(const int argc, const char** argv) {
  ParsedArgs parsed_args;

  std::set<std::string> presented;

  for (int i = 1; i < argc;) {
    std::string name_with_dashes(argv[i]);

    if (!StartsWith(name_with_dashes, "--")) {
      FAIL_PARSE("Argument expected: --{name}, found "
                 << Quoted(name_with_dashes));
    }

    auto name = WithoutDashes(name_with_dashes);

    auto it = args_.find(name);
    if (it == args_.end()) {
      FAIL_PARSE(
          "Unexpected command line argument: " << Quoted(name_with_dashes));
    }

    const auto& argument = it->second;

    if (argument.flag) {
      // Flags: --{name}
      parsed_args.AddFlag(name);
      ++i;
    } else {
      // --{name} {value}
      if (i + 1 >= argc) {
        FAIL_PARSE("Value for command line argument "
                   << Quoted(name_with_dashes) << " not set");
      }
      std::string value{argv[i + 1]};
      i += 2;
      presented.insert(name);
      parsed_args.Add(name, value);
    }
  }

  // Defaults

  for (const auto& [_, argument] : args_) {
    if (!argument.flag && !presented.count(argument.name)) {
      if (argument.default_value.has_value()) {
        parsed_args.Add(argument.name, *argument.default_value);
      } else {
        FAIL_PARSE("Required argument not set: " << Quoted(argument.name));
      }
    }
  }

  return parsed_args;
}

void ArgumentsParser::PrintHelp() {
  std::cout << name_ << " CLI:" << std::endl;
  for (const auto& [_, arg] : args_) {
    if (arg.flag) {
      std::cout << "--" << arg.name << " (FLAG)" << std::endl;
    } else {
      std::cout << "--" << arg.name << " " << arg.value_descr;
      if (arg.default_value.has_value()) {
        std::cout << " (default: " << Quoted(*arg.default_value) << ")";
      }
      std::cout << std::endl;
    }
  }
}

void ArgumentsParser::Fail(const std::string& error) {
  PrintHelp();
  WHEELS_PANIC(error);
}

}  // namespace wheels

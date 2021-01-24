#pragma once

#include <wheels/support/string_builder.hpp>

#include <string>
#include <vector>
#include <map>
#include <set>

namespace wheels {

using Args = std::vector<std::string>;
using NamedArgs = std::map<std::string, std::string>;

class ArgumentsParser {
 public:
  void AddArgument(const std::string& name) {
    arg_long_names_.insert(MakeLongArgument(name));
  }

  NamedArgs Parse(const int argc, const char** argv);

 private:
  // {name} -> --{name}
  static std::string MakeLongArgument(std::string name);
  // --{name} -> {name}
  static std::string GetLongArgumentName(const std::string argument);

 private:
  std::set<std::string> arg_long_names_;
};

}  // namespace wheels

#pragma once

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
    arg_names_.insert(name);
  }

  NamedArgs Parse(const int argc, const char** argv);

 private:
  std::set<std::string> arg_names_;
};

}  // namespace wheels

#pragma once

#include <string>
#include <vector>
#include <map>

namespace wheels {

using Args = std::vector<std::string>;
using NamedArgs = std::map<std::string, std::string>;

NamedArgs AsNamedArgs(const int argc, const char** argv);

}  // namespace wheels

#pragma once

#include <string>
#include <vector>

namespace wheels {

using Tokens = std::vector<std::string>;

Tokens Split(std::string str, char delimiter);

}  // namespace wheels

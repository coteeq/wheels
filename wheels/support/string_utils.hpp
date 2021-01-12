#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace wheels {

using Tokens = std::vector<std::string>;

Tokens Split(std::string str, char delimiter);

bool StartsWith(const std::string& str, std::string_view prefix);

}  // namespace wheels

#pragma once

#include <optional>
#include <string>

namespace wheels {

std::optional<std::string> GetEnvVar(const char* name);

std::optional<std::string> GetUser();

}  // namespace wheels

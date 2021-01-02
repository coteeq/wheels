#pragma once

#include <optional>
#include <string>

namespace wheels {

std::optional<std::string> GetEnvVar(const char* name);

std::string GetEnvVarOr(const char* name, const std::string default_value);

std::optional<std::string> GetUser();

}  // namespace wheels

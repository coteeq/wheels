#include <wheels/support/env.hpp>

namespace wheels {

std::optional<std::string> GetEnvVar(const char* name) {
  char* value = std::getenv(name);
  if (value) {
    return std::string(value);
  } else {
    return std::nullopt;
  }
}

std::string GetEnvVarOr(const char* name, const std::string default_value) {
  return GetEnvVar(name).value_or(default_value);
}

std::optional<std::string> GetUser() {
  return GetEnvVar("USER");
}

}  // namespace wheels

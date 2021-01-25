#pragma once

#include <any>

namespace wheels::test {

std::any GetContextImpl(const std::string& key);

template <typename T>
T GetContext(const std::string& key) {
  return std::any_cast<T>(GetContextImpl(key));
}

void SetContextImpl(const std::string& key, std::any value);

template <typename T>
void SetContext(const std::string& key, T value) {
  SetContextImpl(key, {value});
}

inline bool HasContext(const std::string& key) {
  return wheels::test::GetContextImpl(key).has_value();
}

}  // namespace wheels::test

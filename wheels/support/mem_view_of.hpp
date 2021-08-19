#pragma once

#include <wheels/support/mem_view.hpp>

#include <string>
#include <vector>

namespace wheels {

//////////////////////////////////////////////////////////////////////

template <size_t N>
inline ConstMemView ViewOf(char (&arr)[N]) {
  return {arr, N};
}

inline ConstMemView ViewOf(const char* str) {
  return {str, strlen(str)};
}

inline ConstMemView ViewOf(const std::string& str) {
  return {str.data(), str.length()};
}

inline ConstMemView ViewOf(const std::vector<char>& bytes) {
  return {&bytes[0], bytes.size()};
}

//////////////////////////////////////////////////////////////////////

template <size_t N>
inline MutableMemView MutViewOf(char (&arr)[N]) {
  return {arr, N};
}

inline MutableMemView MutViewOf(std::string& str) {
  return {str.data(), str.length()};
}

inline MutableMemView MutViewOf(std::vector<char>& bytes) {
  return {&bytes[0], bytes.size()};
}

}  // namespace wheels

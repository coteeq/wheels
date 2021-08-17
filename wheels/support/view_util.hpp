#pragma once

#include <wheels/support/memspan.hpp>

#include <string>
#include <vector>

namespace wheels {

//////////////////////////////////////////////////////////////////////

ConstMemView ViewOf(const char* str) {
  return {str, strlen(str)};
}

ConstMemView ViewOf(const std::string& str) {
  return {str.data(), str.length()};
}

//////////////////////////////////////////////////////////////////////

template <size_t N>
MutableMemView MutViewOf(char (&arr)[N]) {
  return {arr, N};
}

MutableMemView MutViewOf(std::string& str) {
  return {str.data(), str.length()};
}

MutableMemView MutViewOf(std::vector<char>& bytes) {
  return {&bytes[0], bytes.size()};
}

}  // namespace wheels
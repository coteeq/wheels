#pragma once

#include <wheels/support/noncopyable.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <ostream>

namespace wheels {

////////////////////////////////////////////////////////////

using Tokens = std::vector<std::string>;

Tokens Split(std::string str, char delimiter);

////////////////////////////////////////////////////////////

bool StartsWith(const std::string& str, std::string_view prefix);

////////////////////////////////////////////////////////////

namespace detail {

struct Quoted {
  std::string str;
};

std::ostream& operator<<(std::ostream& out, const Quoted& quoted);

}  // namespace detail

inline detail::Quoted Quoted(const std::string& str) {
  return detail::Quoted{str};
}

}  // namespace wheels

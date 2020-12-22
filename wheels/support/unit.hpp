#pragma once

namespace wheels {

// https://en.wikipedia.org/wiki/Unit_type
// https://github.com/facebook/folly/blob/master/folly/Unit.h

struct Unit {};

inline bool operator==(const Unit& /*lhs*/, const Unit& /*rhs*/) {
  return true;
}
inline bool operator!=(const Unit& /*lhs*/, const Unit& /*rhs*/) {
  return false;
}

}  // namespace wheels

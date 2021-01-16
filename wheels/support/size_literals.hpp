#pragma once

#include <cstdlib>

namespace wheels::size_literals {

using TCount = unsigned long long;

inline size_t operator""_KiB(TCount count) {
  return count * 1024;
}

inline size_t operator""_MiB(TCount count) {
  return count * 1024_KiB;
}

inline size_t operator""_GiB(TCount count) {
  return (size_t)count * 1024_MiB;
}

}  // namespace wheels::size_literals

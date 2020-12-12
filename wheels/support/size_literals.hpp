#pragma once

#include <cstdlib>

namespace wheels::size_literals {

using TCount = unsigned long long;

size_t operator""_KiB(TCount count) {
  return count * 1024;
}

size_t operator""_MiB(TCount count) {
  return count * 1024 * 1024;
}

size_t operator""_GiB(TCount count) {
  return (size_t)count * 1024 * 1024 * 1024;
}

}  // namespace wheels::size_literals

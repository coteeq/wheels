#include <wheels/support/nullptr.hpp>

#include <wheels/support/compiler.hpp>

std::ostream& operator<<(std::ostream& out, std::nullptr_t _) {
  WHEELS_UNUSED(_);
  return out << "nullptr";
}

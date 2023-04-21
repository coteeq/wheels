#include <wheels/core/source_location.hpp>

namespace wheels {

// Format: {file}:{function}[Line {line_number}]

std::ostream& operator<<(std::ostream& out, const SourceLocation& where) {
  out << where.File() << ":" << where.Line() << " at " << where.Function();
  return out;
}

}  // namespace wheels

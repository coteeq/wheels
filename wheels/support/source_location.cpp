#include <wheels/support/source_location.hpp>

////////////////////////////////////////////////////////////////////////////////

// Format: {file}:{function}[Line {line_number}]

std::ostream& operator<<(std::ostream& out, const SourceLocation& where) {
  out << where.file_ << ":" << where.function_ << "[Line " << where.line_
      << "]";
  return out;
}

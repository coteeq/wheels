#pragma once

#include <iostream>
#include <string_view>

////////////////////////////////////////////////////////////////////////////////

// Represents location in source code: file name, function name, line number

struct SourceLocation {
  std::string_view file_;
  std::string_view function_;
  int line_;

  SourceLocation(std::string_view file, std::string_view function, int line)
      : file_(file), function_(function), line_(line) {
  }

  SourceLocation() : line_(0) {
  }
};

std::ostream& operator<<(std::ostream& out, const SourceLocation& where);

// Use HERE in macros to capture current location

#define HERE() SourceLocation(__FILE__, __PRETTY_FUNCTION__, __LINE__)

#include <wheels/test/helpers.hpp>

#include <algorithm>

namespace wheels {

static bool IsMultiLine(const std::string& error) {
  return std::count(error.begin(), error.end(), '\n') > 1;
}

std::string FormatStderrForErrorMessage(const std::string& stderr) {
  if (stderr.empty()) {
    return "<empty>";
  }
  if (!IsMultiLine(stderr)) {
    return stderr;
  }
  // Multi-line stderr
  return std::string("\n") + stderr + "\n";
}

}  // namespace wheels

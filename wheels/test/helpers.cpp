#include <wheels/test/helpers.hpp>

#include <wheels/support/string_builder.hpp>

#include <algorithm>
#include <iomanip>

namespace wheels::test {

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

double ToSeconds(wheels::Duration elapsed) {
  return std::chrono::duration<double>(elapsed).count();
}

std::string FormatSeconds(const Duration d, size_t digits) {
  return StringBuilder() << std::fixed << std::setprecision(digits)
                         << ToSeconds(d);
}

}  // namespace wheels::test

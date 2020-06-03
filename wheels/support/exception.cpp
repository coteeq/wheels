#include <wheels/support/exception.hpp>

#include <wheels/support/assert.hpp>

#include <stdexcept>

namespace wheels {

std::string CurrentExceptionMessage() {
  auto current = std::current_exception();

  WHEELS_VERIFY(current, "Not in exception context");

  try {
    std::rethrow_exception(current);
  } catch (std::exception& e) {
    return {e.what()};
  } catch (...) {
    return "Wild exception";
  }
}

}  // namespace tiny::support

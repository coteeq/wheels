#include <wheels/support/panic.hpp>

#include <mutex>

namespace detail {

void Panic(const std::string& error) {
  static std::mutex mutex;

  {
    std::lock_guard guard(mutex);
    std::cerr << error << std::endl;
  }

  std::abort();
}

}  // namespace detail

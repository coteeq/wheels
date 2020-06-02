#include <wheels/support/panic.hpp>

#include <wheels/support/locking.hpp>

namespace detail {

void Panic(const std::string& error) {
  static std::mutex mutex;

  {
    auto guard = wheels::LockUnique(mutex);
    std::cerr << error << std::endl;
  }

  std::abort();
}

}  // namespace detail

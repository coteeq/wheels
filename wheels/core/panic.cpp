#include <wheels/core/panic.hpp>

#include <iostream>
#include <mutex>
#include "wheels/logging/logging.hpp"

namespace wheels {

static std::mutex mutex;

void Panic(const std::string& error, SourceLocation where) {
  {
    std::lock_guard guard(mutex);
    wheels::FlushPendingLogMessages();
    std::cerr << "Panicked at " << where << ": " << error << std::endl;
  }

  std::abort();
}

}  // namespace wheels

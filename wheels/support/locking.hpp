#pragma once

#include <mutex>
#include <shared_mutex>

namespace wheels {

// Usage: auto lock = LockUnique(mutex);
// Replacement for std::unique_lock<std::mutex> lock(mutex);

template <class Mutex>
std::unique_lock<Mutex> LockUnique(Mutex& mutex) {
  return std::unique_lock<Mutex>{mutex};
}

template <class RWMutex>
std::shared_lock<RWMutex> LockShared(RWMutex& rwmutex) {
  return std::shared_lock<RWMutex>{rwmutex};
}

}  // namespace wheels

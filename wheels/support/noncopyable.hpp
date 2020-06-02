#pragma once

namespace wheels {

struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable& that) = delete;
  NonCopyable& operator=(const NonCopyable& that) = delete;
};

}  // namespace twist

#pragma once

#include <wheels/support/memspan.hpp>
#include <wheels/support/noncopyable.hpp>

namespace wheels {

class MmapAllocation : public NonCopyable {
 public:
  MmapAllocation() {
    Reset();
  }

  static size_t PageSize();

  // Allocate `count` pages of zeroed memory
  static MmapAllocation AllocatePages(size_t count);

  // Moving
  MmapAllocation(MmapAllocation&& that);
  MmapAllocation& operator=(MmapAllocation&& that);

  ~MmapAllocation() {
    Release();
  }

  char* Start() const {
    return start_;
  }

  char* End() const {
    return start_ + size_;
  }

  size_t Size() const {
    return size_;
  }

  ConstMemView View() const {
    return {start_, size_};
  }

  MutableMemView View() {
    return {start_, size_};
  }

  MemSpan AsMemSpan() {
    return View();
  }

  // Protect range of pages
  // Protected pages cannot be read, written or executed
  void ProtectPages(size_t offset, size_t count);

  void Release();

 private:
  MmapAllocation(char* start, size_t size) : start_(start), size_(size) {
  }

  void Reset();

 private:
  // Aligned to page boundary
  char* start_;
  size_t size_;
};

}  // namespace wheels

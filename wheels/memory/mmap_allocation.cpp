#include <wheels/memory/mmap_allocation.hpp>

#include <wheels/support/assert.hpp>

#include <cerrno>
#include <cstring>

#include <unistd.h>

#include <sys/mman.h>

namespace wheels {

#define CHECK_RESULT(ret, error)                                  \
  WHEELS_VERIFY(ret != -1, error << " (errno = " << errno << ", " \
                                 << strerror(errno) << ")")

//////////////////////////////////////////////////////////////////////

class PageSizeDetector {
 public:
  PageSizeDetector() {
    page_size_ = DetectPageSize();
  }

  size_t GetPageSize() const {
    return page_size_;
  }

 private:
  size_t DetectPageSize() {
    return sysconf(_SC_PAGESIZE);
  }

 private:
  size_t page_size_;
};

static const PageSizeDetector page_size_detector;

//////////////////////////////////////////////////////////////////////

static size_t PagesToBytes(size_t count) {
  return count * page_size_detector.GetPageSize();
}

size_t MmapAllocation::PageSize() {
  return page_size_detector.GetPageSize();
}

MmapAllocation MmapAllocation::AllocatePages(size_t count) {
  size_t size = PagesToBytes(count);

  void* start = mmap(/*addr=*/nullptr, /*length=*/size,
                     /*prot=*/PROT_READ | PROT_WRITE,
                     /*flags=*/MAP_PRIVATE | MAP_ANONYMOUS,
                     /*fd=*/-1, /*offset=*/0);

  WHEELS_VERIFY(start != MAP_FAILED, "Cannot allocate " << count << " pages");

  return MmapAllocation{(char*)start, size};
}

void MmapAllocation::ProtectPages(size_t offset, size_t count) {
  int ret = mprotect(/*addr=*/(void*)(start_ + PagesToBytes(offset)),
                     /*len=*/PagesToBytes(count),
                     /*prot=*/PROT_NONE);
  CHECK_RESULT(
      ret, "Cannot protect pages [" << offset << ", " << offset + count << ")");
}

MmapAllocation::MmapAllocation(MmapAllocation&& that) {
  start_ = that.start_;
  size_ = that.size_;
  that.Reset();
}

MmapAllocation& MmapAllocation::operator=(MmapAllocation&& that) {
  Release();
  start_ = that.start_;
  size_ = that.size_;
  that.Reset();
  return *this;
}

void MmapAllocation::Release() {
  if (start_ == nullptr) {
    return;
  }

  int ret = munmap((void*)start_, size_);
  CHECK_RESULT(ret, "Cannot unmap allocated pages");
}

void MmapAllocation::Reset() {
  start_ = nullptr;
  size_ = 0;
}

}  // namespace wheels

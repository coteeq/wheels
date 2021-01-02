#include <wheels/support/mmap_allocation.hpp>

#include <wheels/test/test_framework.hpp>

using wheels::MmapAllocation;

TEST_SUITE(Mmap) {
  SIMPLE_TEST(PageSize) {
    ASSERT_EQ(MmapAllocation::PageSize(), 4096);
  }

  SIMPLE_TEST(CheckPages) {
    static const size_t kPages = 3;

    const size_t page_size = MmapAllocation::PageSize();

    auto alloc = MmapAllocation::AllocatePages(kPages);
    ASSERT_EQ(alloc.Size(), page_size * kPages);

    for (size_t i = 0; i < kPages; ++i) {
      *(alloc.Start() + i * page_size) = '!';
    }
  }

  SIMPLE_TEST(View) {
    auto alloc = MmapAllocation::AllocatePages(3);
    auto view = alloc.AsMemSpan();

    ASSERT_EQ(alloc.Start(), view.Data());
    ASSERT_EQ(alloc.Size(), view.Size());
  }
}
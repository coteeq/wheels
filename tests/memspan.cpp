#include <wheels/support/mmap_allocation.hpp>
#include <wheels/support/memspan.hpp>

#include <wheels/test/test_framework.hpp>

using wheels::MmapAllocation;
using wheels::MutableMemView;
using wheels::ConstMemView;
using wheels::ViewOf;

TEST_SUITE(MemSpan) {
  SIMPLE_TEST(DefaultCtor) {
    MutableMemView mut_view{};
    ASSERT_FALSE(mut_view.IsValid());

    ConstMemView const_view{};
    ASSERT_FALSE(const_view.IsValid());
  }

  SIMPLE_TEST(Ops) {
    auto alloc = MmapAllocation::AllocatePages(1);

    ConstMemView const_view = alloc.View();

    ASSERT_TRUE(const_view.IsValid());

    ASSERT_EQ(const_view.Data(), alloc.Start());
    ASSERT_EQ(const_view.Size(), alloc.Size());
    ASSERT_EQ(const_view.Begin(), alloc.Start());
    ASSERT_FALSE(const_view.IsEmpty());
  }

  SIMPLE_TEST(ViewOf) {
    static const size_t kLength = 15;
    char data[kLength];

    auto mut_view = wheels::ViewOf(data);
    ASSERT_TRUE(mut_view.IsValid());
    ASSERT_EQ(mut_view.Data(), data);
    ASSERT_EQ(mut_view.Size(), kLength);
  }

  SIMPLE_TEST(Offset) {
    static const size_t kLength = 15;
    char data[kLength];

    auto mut_view = ViewOf(data);

    size_t bytes_read = 0;
    while (mut_view.HasSpace()) {
      mut_view += 1;  // Read some
      bytes_read += 1;
    }
  }
}

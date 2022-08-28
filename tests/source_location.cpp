#include <wheels/test/test_framework.hpp>

using wheels::SourceLocation;

TEST_SUITE(SourceLocation) {
  int Foo(SourceLocation loc = SourceLocation::Current()) {
    return loc.Line();
  }

  SIMPLE_TEST(Current) {
    ASSERT_EQ(Foo(), 11);
  }
}

#include <wheels/memory/buffer.hpp>

#include <wheels/test/test_framework.hpp>

#include <wheels/io/read.hpp>
#include <wheels/io/memory.hpp>

#include <memory>

TEST_SUITE(GrowingBuffer) {
  // Helper
  std::string ReadAllFrom(const wheels::GrowingBuffer& buffer) {
    wheels::io::MemoryReader reader(buffer.View());
    auto all = wheels::io::ReadAll(&reader);
    return *all;
  }

  SIMPLE_TEST(JustWorks) {
    wheels::GrowingBuffer buffer;

    buffer.Append("Hello");
    buffer.Append(", ");
    buffer.Append("World!");

    ASSERT_EQ(ReadAllFrom(buffer), "Hello, World!");
  }

  SIMPLE_TEST(Clear) {
    wheels::GrowingBuffer buffer;

    buffer.Append("Hello");
    buffer.Append("!");

    ASSERT_EQ(ReadAllFrom(buffer), "Hello!");

    buffer.Clear();

    buffer.Append("Test");
    buffer.Append("123");

    ASSERT_EQ(ReadAllFrom(buffer), "Test123");
  }
}
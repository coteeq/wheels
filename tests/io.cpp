#include <wheels/io/read.hpp>
#include <wheels/io/write.hpp>
#include <wheels/io/memory.hpp>
#include <wheels/io/limit.hpp>

#include <wheels/support/view_util.hpp>

#include <wheels/test/test_framework.hpp>

namespace wheels::io {

class TestReader : public IReader {
 public:
  TestReader(IReader* from) : from_(from) {
  }

  size_t ReadSome(MutableMemView buffer) {
    if (!buffer.HasSpace()) {
      return 0;
    }
    size_t bytes_read = from_->ReadSome({buffer.Data(), 1});
    return bytes_read;
  }

 private:
  IReader* from_;
};

class FramedReader {
 public:
  FramedReader(IReader* from) : from_(from) {
  }

  std::string NextFrame(size_t length) {
    LimitReader frame_reader(from_, length);
    auto frame = ReadAll(&frame_reader);
    WHEELS_VERIFY(frame.length() == length, "Cannot read frame: expected " << length << " bytes, read " << frame.length());
    return frame;
  }

 private:
  IReader* from_;
};

}  // namespace wheels::io

TEST_SUITE(IO) {
  SIMPLE_TEST(ReadAll) {
    static const std::string kMessage = "Hello, World!";
    wheels::io::MemoryReader mem_reader(wheels::ViewOf(kMessage));
    wheels::io::TestReader test_reader(&mem_reader);

    auto message = wheels::io::ReadAll(&test_reader);
    ASSERT_EQ(message, kMessage);
  }

  SIMPLE_TEST(LimitReader) {
    static const std::string kFrames = "FirstSecondThird";

    wheels::io::MemoryReader mem_reader(wheels::ViewOf(kFrames));
    wheels::io::FramedReader framed_reader(&mem_reader);

    ASSERT_EQ(framed_reader.NextFrame(5), "First");
    ASSERT_EQ(framed_reader.NextFrame(6), "Second");
    ASSERT_EQ(framed_reader.NextFrame(5), "Third");
  }
}
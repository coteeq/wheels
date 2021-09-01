#include <wheels/io/read.hpp>
#include <wheels/io/write.hpp>
#include <wheels/io/memory.hpp>
#include <wheels/io/limit.hpp>
#include <wheels/io/string.hpp>
#include <wheels/io/buffered.hpp>

#include <wheels/memory/view_of.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>

using wheels::io::IReader;
using wheels::io::IWriter;

using wheels::ConstMemView;
using wheels::MutableMemView;

using wheels::ViewOf;
using wheels::MutViewOf;

namespace test {

//////////////////////////////////////////////////////////////////////

class ChunkedReader : public IReader {
 public:
  ChunkedReader(IReader* from, size_t chunk_size) : from_(from), chunk_size_(chunk_size) {
  }

  size_t ReadSome(MutableMemView buffer) {
    if (!buffer.HasSpace()) {
      return 0;
    }
    size_t bytes_to_read = BytesToRead(buffer);
    return from_->ReadSome({buffer.Data(), bytes_to_read});
  }

 private:
  size_t BytesToRead(MutableMemView buffer) const {
    if (buffer.Size() < chunk_size_) {
      return buffer.Size();
    } else {
      return chunk_size_;
    }
  }

 private:
  IReader* from_;
  size_t chunk_size_;
};

//////////////////////////////////////////////////////////////////////

class AtLeastReader : public IReader {
 public:
  AtLeastReader(IReader* from, size_t threshold) : from_(from), threshold_(threshold) {
  }

  size_t ReadSome(MutableMemView buffer) override {
    WHEELS_VERIFY(buffer.Size() >= threshold_, "Too small input buffer: " << buffer.Size());
    return from_->ReadSome(buffer);
  }

 private:
  IReader* from_;
  size_t threshold_;
};

//////////////////////////////////////////////////////////////////////

class FrameReader {
 public:
  FrameReader(IReader* from) : from_(from) {
  }

  std::string NextFrame(size_t length) {
    wheels::io::LimitReader frame_reader(from_, length);
    auto frame = ReadAll(&frame_reader);
    WHEELS_VERIFY(frame.length() == length, "Cannot read frame: expected " << length << " bytes, read " << frame.length());
    return frame;
  }

 private:
  IReader* from_;
};

}  // namespace test

TEST_SUITE(IO) {
  SIMPLE_TEST(Copy) {
    static const std::string kSource = "Hello, World!";

    wheels::io::MemoryReader source_reader(wheels::ViewOf(kSource));

    std::string dest;
    wheels::io::StringWriter dest_writer(dest);

    char buf[3];

    size_t bytes_read = wheels::io::CopyAll(
        &source_reader,
        &dest_writer,
        wheels::MutViewOf(buf));

    ASSERT_EQ(bytes_read, kSource.length());
    ASSERT_EQ(dest, kSource);
  }

  SIMPLE_TEST(ReadAll) {
    static const std::string kSource = "Hello, World!";

    wheels::io::MemoryReader mem_reader(wheels::ViewOf(kSource));
    test::ChunkedReader chunked_reader(&mem_reader, /*chunk_size=*/1);

    auto dest = wheels::io::ReadAll(&chunked_reader);
    ASSERT_EQ(dest, kSource);
  }

  SIMPLE_TEST(LimitReader) {
    static const std::string kSource = "FirstSecondThird";

    wheels::io::MemoryReader mem_reader(wheels::ViewOf(kSource));
    test::FrameReader frame_reader(&mem_reader);

    ASSERT_EQ(frame_reader.NextFrame(5), "First");
    ASSERT_EQ(frame_reader.NextFrame(6), "Second");
    ASSERT_EQ(frame_reader.NextFrame(5), "Third");
  }

  void TestBufferedReader(size_t buf_size) {
    static const std::string kSource = "012345678901234567890134";

    // Read pipeline: Buffered <- Stat <- Memory <- kSource
    wheels::io::MemoryReader source_reader(wheels::ViewOf(kSource));
    test::AtLeastReader at_least_reader(&source_reader, buf_size);
    wheels::io::BufferedReader buffered_reader(&at_least_reader, buf_size);

    // String -> output
    std::string dest;
    wheels::io::StringWriter dest_writer(dest);

    char buf[1];
    wheels::io::CopyAll(
        &buffered_reader,
        &dest_writer,
        wheels::MutViewOf(buf));

    ASSERT_EQ(kSource, dest);
  }

  SIMPLE_TEST(BufferedReader1) {
    TestBufferedReader(5);
  }

  SIMPLE_TEST(BufferedReader2) {
    TestBufferedReader(2);
  }

  SIMPLE_TEST(BufferedReader3) {
    TestBufferedReader(1024);
  }
}

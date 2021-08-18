#include <wheels/io/read.hpp>

#include <wheels/io/string.hpp>

#include <wheels/support/view_util.hpp>

namespace wheels::io {

size_t Read(IReader* reader, MutableMemView buffer) {
  size_t total_bytes_read = 0;

  while (buffer.HasSpace()) {
    const size_t bytes_read = reader->ReadSome(buffer);
    if (bytes_read == 0) {
      break;
    }
    buffer += bytes_read;
    total_bytes_read += bytes_read;
  }

  return total_bytes_read;
}

std::string ReadAll(IReader* reader) {
  std::string all;
  StringWriter all_writer(all);

  char buffer[1024];

  while (true) {
    size_t bytes_read = reader->ReadSome(MutViewOf(buffer));
    if (bytes_read == 0) {
      break;
    }
    all_writer.Write({buffer, bytes_read});
  }

  return all;
}

}  // namespace wheels::io

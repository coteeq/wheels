#include <wheels/io/read.hpp>

#include <wheels/io/string.hpp>

#include <wheels/support/mem_view_of.hpp>

namespace wheels::io {

size_t Read(IReader* from, MutableMemView buffer) {
  size_t total_bytes_read = 0;

  while (buffer.HasSpace()) {
    const size_t bytes_read = from->ReadSome(buffer);
    if (bytes_read == 0) {
      break;
    }
    buffer += bytes_read;
    total_bytes_read += bytes_read;
  }

  return total_bytes_read;
}

size_t CopyAll(IReader* from, IWriter* to, MutableMemView buffer) {
  size_t total = 0;
  while (true) {
    size_t bytes_read = from->ReadSome(buffer);
    if (bytes_read == 0) {
      break;
    }
    total += bytes_read;
    to->Write({buffer.Begin(), bytes_read});
  }
  return total;
}

std::string ReadAll(IReader* from) {
  std::string all;
  StringWriter all_writer(all);

  char buffer[1024];

  CopyAll(from, /*to=*/&all_writer, MutViewOf(buffer));
  return all;
}

}  // namespace wheels::io

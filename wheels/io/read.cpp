#include <wheels/io/read.hpp>

#include <wheels/io/string.hpp>

#include <wheels/result/make.hpp>
#include <wheels/memory/view_of.hpp>

using namespace wheels::make_result;

namespace wheels::io {

Result<size_t> Read(IReader* from, MutableMemView buffer) {
  size_t total_bytes_read = 0;

  while (buffer.HasSpace()) {
    const auto bytes_read = from->ReadSome(buffer);
    if (!bytes_read.IsOk()) {
      return PropagateError(bytes_read);
    }
    if (*bytes_read == 0) {
      break;
    }
    buffer += *bytes_read;
    total_bytes_read += *bytes_read;
  }

  return Ok(total_bytes_read);
}

Result<size_t> CopyAll(IReader* from, IWriter* to, MutableMemView buffer) {
  size_t total = 0;
  while (true) {
    auto bytes_read = from->ReadSome(buffer);
    if (!bytes_read.IsOk()) {
      return PropagateError(bytes_read);
    }
    if (*bytes_read == 0) {
      break;
    }
    total += *bytes_read;

    auto written = to->Write({buffer.Begin(), *bytes_read});
    if (!written.IsOk()) {
      return PropagateError(written);
    }
  }
  return Ok(total);
}

Result<std::string> ReadAll(IReader* from) {
  std::string all;
  StringWriter all_writer(all);

  char buffer[1024];

  auto copied = CopyAll(from, /*to=*/&all_writer, MutViewOf(buffer));
  if (!copied.IsOk()) {
    return PropagateError(copied);
  }
  return Ok(all);
}

}  // namespace wheels::io

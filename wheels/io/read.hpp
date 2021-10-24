#pragma once

#include <wheels/io/reader.hpp>
#include <wheels/io/writer.hpp>

#include <string>

namespace wheels::io {

Result<size_t> Read(IReader* from, MutableMemView buffer);

Result<size_t> CopyAll(IReader* from, IWriter* to, MutableMemView buffer);

Result<std::string> ReadAll(IReader* from);

}  // namespace wheels::io

#pragma once

#include <wheels/io/reader.hpp>
#include <wheels/io/writer.hpp>

#include <string>

namespace wheels::io {

size_t Read(IReader* from, MutableMemView buffer);

size_t CopyAll(IReader* from, IWriter* to, MutableMemView buffer);

std::string ReadAll(IReader* from);

}  // namespace wheels::io

#pragma once

#include <wheels/io/reader.hpp>

#include <string>

namespace wheels::io {

size_t Read(IReader* reader, MutableMemView buffer);

std::string ReadAll(IReader* reader);

}  // namespace wheels::io
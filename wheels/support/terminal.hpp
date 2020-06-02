#pragma once

#include <string_view>

namespace terminal {

std::string_view Red();
std::string_view Green();
std::string_view Yellow();
std::string_view Cyan();
std::string_view Magenta();
std::string_view Reset();

}  // namespace terminal

#define GREEN(smth) terminal::Green() << smth << terminal::Reset()
#define RED(smth) terminal::Red() << smth << terminal::Reset()
#define CYAN(smth) terminal::Cyan() << smth << terminal::Reset()

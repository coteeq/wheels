#pragma once

#include <wheels/support/time.hpp>

#include <string>

namespace wheels::test {

// TODO: better
std::string FormatStderrForErrorMessage(const std::string& stderr);

double ToSeconds(const Duration d);

std::string FormatSeconds(const Duration d);

}  // namespace wheels::test

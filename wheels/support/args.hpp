#pragma once

#include <wheels/support/string_builder.hpp>

#include <wheels/support/assert.hpp>
#include <wheels/support/string_utils.hpp>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <optional>

namespace wheels {

////////////////////////////////////////////////////////////////////////////////

class ParsedArgs {
 public:
  std::string Get(const std::string& name) const {
    if (auto it = values_.find(name); it != values_.end()) {
      return it->second;
    }
    WHEELS_PANIC("Argument " << Quoted(name) << " not found");
  }

  bool HasFlag(const std::string& name) const {
    return flags_.count(name) > 0;
  }

  // Build

  void Add(const std::string& name, const std::string& value) {
    values_.insert_or_assign(name, value);
  }

  void AddFlag(const std::string& name) {
    flags_.insert(name);
  }

 private:
  std::set<std::string> flags_;
  std::map<std::string, std::string> values_;
};

////////////////////////////////////////////////////////////////////////////////

struct Argument {
  std::string name;
  bool flag{false};
  std::optional<std::string> default_value{};
  std::string value_descr{"value"};

  Argument& Flag() {
    WHEELS_VERIFY(!default_value.has_value(), "Inconsistent");
    flag = true;
    value_descr = "";
    return *this;
  }

  Argument& WithDefault(std::string value) {
    WHEELS_VERIFY(!flag, "Inconsistent");
    default_value = value;
    return *this;
  }

  Argument& ValueDescr(std::string descr) {
    WHEELS_VERIFY(!flag, "Inconsistent");
    value_descr = descr;
    return *this;
  }
};

////////////////////////////////////////////////////////////////////////////////

class ArgumentsParser {
 public:
  ArgumentsParser(const std::string& name) : name_(name) {
  }

  void Add(const std::string& name) {
    Add(Argument{name});
  }

  void Add(const Argument& argument) {
    args_.insert_or_assign(argument.name, argument);
  }

  void AddFlag(const std::string& name) {
    Add(Argument{name}.Flag());
  }

  ParsedArgs Parse(const int argc, const char** argv);

 private:
  void PrintHelp();
  void Fail(const std::string& error);

  // --{name} -> {name}
  static std::string WithoutDashes(const std::string argument);

 private:
  std::string name_;
  std::map<std::string, Argument> args_;
};

}  // namespace wheels

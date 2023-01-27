#pragma once

#include <wheels/core/string_builder.hpp>
#include <wheels/core/assert.hpp>
#include <wheels/core/noncopyable.hpp>

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

  bool Has(const std::string& name) const {
    return values_.count(name) > 0;
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
  bool optional{false};
  std::optional<std::string> help;
};

////////////////////////////////////////////////////////////////////////////////

class ArgumentParser;

struct ArgumentBuilder : public wheels::NonCopyable {
  using Builder = ArgumentBuilder;

  ArgumentBuilder(ArgumentParser* parser, std::string name) : parser_(parser) {
    arg_.name = name;
  }

  ~ArgumentBuilder();

  Builder& Flag() {
    WHEELS_VERIFY(!arg_.default_value.has_value(), "Inconsistent");
    arg_.flag = true;
    arg_.value_descr = "";
    return *this;
  }

  Builder& WithDefault(std::string value) {
    WHEELS_VERIFY(!arg_.flag, "Inconsistent");
    arg_.default_value = value;
    return *this;
  }

  Builder& ValueDescr(std::string descr) {
    WHEELS_VERIFY(!arg_.flag, "Inconsistent");
    arg_.value_descr = descr;
    return *this;
  }

  Builder& Optional() {
    arg_.optional = true;
    return *this;
  }

  Builder& Help(std::string help) {
    arg_.help = help;
    return *this;
  }

 private:
  ArgumentParser* parser_;
  Argument arg_;
};

////////////////////////////////////////////////////////////////////////////////

class ArgumentParser {
 public:
  ArgumentParser(const std::string& name) : name_(name) {
  }

  void Add(const Argument& argument);

  ArgumentBuilder Add(const std::string& name) {
    return {this, name};
  }

  void AddHelpFlag(const std::string& help = "help") {
    Add(help).Flag().Help("Print help and exit");
    help_flag_ = help;
  }

  ParsedArgs Parse(const int argc, const char** argv);

 private:
  void PrintHelp();
  void Fail(const std::string& error);

  // --{name} -> {name}
  static std::string WithoutDashes(const std::string& argument);

 private:
  std::string name_;
  std::map<std::string, Argument> args_;
  std::optional<std::string> help_flag_;
};

}  // namespace wheels

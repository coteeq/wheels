#pragma once

#include <functional>
#include <memory>
#include <string>

namespace wheels {

////////////////////////////////////////////////////////////////////////////////

class IByteStreamConsumer {
 public:
  virtual ~IByteStreamConsumer() = default;
  virtual void Consume(const char* buf, size_t length) = 0;
  virtual void Eof() = 0;
};

using IByteStreamConsumerPtr = std::unique_ptr<IByteStreamConsumer>;

////////////////////////////////////////////////////////////////////////////////

class Result {
 public:
  Result(int status, std::string stdout, std::string stderr)
      : status_(status),
        stdout_(std::move(stdout)),
        stderr_(std::move(stderr)) {
  }

  bool Exited(int& exit_code) const;
  bool KilledBySignal(int& signal) const;

  const std::string& GetStdout() const {
    return stdout_;
  }

  const std::string& GetStderr() const {
    return stderr_;
  }

 private:
  int status_;
  std::string stdout_;
  std::string stderr_;
};

////////////////////////////////////////////////////////////////////////////////

using TargetFunction = std::function<void()>;

Result ExecuteWithFork(TargetFunction target,
                       IByteStreamConsumerPtr stdout_consumer,
                       IByteStreamConsumerPtr stderr_consumer);

}  // namespace wheels

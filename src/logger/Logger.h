#pragma once

#include "../ThreadPool.h"

#include <chrono>
#include <cstring>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

namespace sf {

// There is a class called Logger and it should be singleton ??

// It allow user to setup following info:
// 1. Allow user to define format message
// 2. Allow user to set log to console / file
// 3. Log rotation ??

enum LogTarget { Console = 1, File = 1 << 1 };

enum class LogLevel { Debug, Info, Warn, Error, Critical };

std::string LogLevelToStr(LogLevel logLevel);

struct LoggerSetting {
  /*
   * %datetime: Write datetime info to the log
   * %logLevel: Write log level to the log
   * %tag: Write tag to the log, if tag is empty it is skipped
   * %threadId: Write thread id to the log
   * %file: Write source file name to the log
   * %line: Write source line number to the log
   * %function: Write caller function to the log
   * %logMsg: Write the log message defined by user to the log
   */
  std::string logMsgFormat =
      "%datetime [%logLevel][%tag][%threadId] [%file:%line][%function] %logMsg";

  // The format of dateTimeFormat should follow std::format (C++20) or older
  // strftime
  std::string dateTimeFormat = "%Y-%m-%d %H:%M:%S";

  int logTargets = LogTarget::Console;

  std::string logFileName;
};

class Logger {
public:
  static Logger *getInstance();

  void setLoggerSetting(const LoggerSetting &settings);

  template <typename... Args>
  void log(LogLevel logLevel, const char *logTag, const char *formatMsg,
           Args... args) {
    // Immediate add log to queue then format message and write log there for
    // better performance
    auto threadId = std::this_thread::get_id();
    auto now = std::chrono::system_clock::now();
    _pool.addTask([this, threadId, now, logLevel, logTag, formatMsg,
                   ... capturedArgs = std::forward<Args>(args)]() {
      std::string fullLog = _settings.logMsgFormat;
      if (size_t pos = fullLog.find("%datetime"); pos != std::string::npos) {
        std::string fmt = "{:" + _settings.dateTimeFormat + "}";
        std::string dt = std::vformat(fmt, std::make_format_args(now));
        fullLog.replace(pos, std::strlen("%datetime"), dt);
      }

      if (size_t pos = fullLog.find("%logLevel"); pos != std::string::npos) {
        fullLog.replace(pos, std::strlen("%logLevel"), LogLevelToStr(logLevel));
      }

      if (size_t pos = fullLog.find("%tag"); pos != std::string::npos) {
        fullLog.replace(pos, std::strlen("%tag"), logTag);
      }

      if (size_t pos = fullLog.find("%threadId"); pos != std::string::npos) {
        std::stringstream ss;
        ss << threadId;
        fullLog.replace(pos, std::strlen("%threadId"), ss.str());
      }

      if (size_t pos = fullLog.find("%logMsg"); pos != std::string::npos) {
        std::string logMsg =
            std::vformat(formatMsg, std::make_format_args(capturedArgs...));

        fullLog.replace(pos, std::strlen("%logMsg"), logMsg);
      }

      if (_settings.logTargets & LogTarget::Console) {
        std::cout << fullLog << '\n';
      }
    });
  }

private:
  Logger() = default;

  Logger(const Logger &other) = delete;

  Logger &operator=(const Logger &other) = delete;

  LoggerSetting _settings;

  // For simplicity, first I will start using a ThreadPool with 1 thread
  ThreadPool _pool{1};
};

template <typename... Args>
void logDebug(const char *tag, const char *formatMsg, Args... args) {
  Logger *logger = Logger::getInstance();
  logger->log(LogLevel::Info, tag, formatMsg, args...);
}

template <typename... Args>
void logInfo(const char *tag, const char *formatMsg, Args... args) {
  Logger *logger = Logger::getInstance();
  logger->log(LogLevel::Info, tag, formatMsg, args...);
}

} // namespace sf
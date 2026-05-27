#include "Logger.h"

sf::Logger *sf::Logger::getInstance() {
  static Logger instance;
  return &instance;
}

void sf::Logger::setLoggerSetting(const LoggerSetting &settings) {
  _settings = settings;
}

std::string sf::LogLevelToStr(LogLevel logLevel) {
  switch (logLevel) {
  case sf::LogLevel::Debug:
    return "DEBUG";
  case sf::LogLevel::Info:
    return "INFO";
  case sf::LogLevel::Warn:
    return "WARN";
  case sf::LogLevel::Critical:
    return "CRITICAL";
  case sf::LogLevel::Error:
    return "ERROR";
  default:
    return "";
  }
}
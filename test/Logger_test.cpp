#include "../src/logger/Logger.h"

#include <gtest/gtest.h>

TEST(LoggerTest, WriteConsoleLogInOneThread) {
  sf::logDebug("WriteConsoleLogInOneThread", "Hello sf Logger");
}
#include "../src/Stopwatch.h"

#include <gtest/gtest.h>
#include <chrono>
#include <iostream>


TEST(StopwatchTest, StartStopWatchByConstructor)
{
  sf::Stopwatch stopwatch;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  stopwatch.stop();

  auto elapsedTime = stopwatch.elapsedTime<>();
  std::cout << "Elapsed time: " << elapsedTime << " seconds" << std::endl;
  EXPECT_GT(elapsedTime, 0);
}

TEST(StopwatchTest, CallStartToStartStopwatch)
{
  sf::Stopwatch stopwatch;
  stopwatch.start();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  stopwatch.stop();

  long long elapsedTime = stopwatch.elapsedTime<std::milli>();
  std::cout << "Elapsed time: " << elapsedTime << " ms" << std::endl;
  EXPECT_GT(elapsedTime, 0);
}

TEST(StopwatchTest, StopWithoutStart)
{
  sf::Stopwatch stopwatch;
  bool ret = stopwatch.stop();
  EXPECT_EQ(ret, true);

  ret = stopwatch.stop();
  EXPECT_EQ(ret, false);
}
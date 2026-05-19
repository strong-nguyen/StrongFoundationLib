#include "../src/Timer.h"

#include <gtest/gtest.h>
#include <chrono>


TEST(TimerTest, StartOneTimeTimer)
{
  sf::Timer oneTimeTimer(sf::TimerType::OneTime);
  int a = 0;
  oneTimeTimer.start(1000, [&a]()
    {
      a = 100;
    });

  std::this_thread::sleep_for(std::chrono::seconds(2));
  EXPECT_EQ(a, 100);
}

TEST(TimerTest, StartStopOneTimeTimer)
{
  sf::Timer oneTimeTimer(sf::TimerType::OneTime);
  int a = 0;
  oneTimeTimer.start(1000, [&a]()
    {
      a = 100;
    });

  std::this_thread::sleep_for(std::chrono::seconds(2));
  oneTimeTimer.stop();
  EXPECT_EQ(a, 100);
}

TEST(TimerTest, StartStopStartOneTimeTimer)
{
  sf::Timer oneTimeTimer(sf::TimerType::OneTime);
  int a = 0;
  oneTimeTimer.start(1000, [&a]()
    {
      a = 100;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  oneTimeTimer.stop();

  oneTimeTimer.start(1000, [&a]()
    {
      a = 200;
    });

  std::this_thread::sleep_for(std::chrono::seconds(2));
  oneTimeTimer.stop();
  EXPECT_EQ(a, 200);
}

TEST(TimerTest, StartPeriodTimer)
{
  int a = 0;
  sf::Timer periodTimer;
  periodTimer.start(500, [&a]()
    {
      a += 100;
    });

  std::this_thread::sleep_for(std::chrono::seconds(2));
  EXPECT_GT(a, 100);
}

TEST(TimerTest, StartStopPeriodTimer)
{
  int a = 0;
  sf::Timer periodTimer;
  periodTimer.start(500, [&a]()
    {
      a += 100;
    });

  std::this_thread::sleep_for(std::chrono::seconds(2));
  periodTimer.stop();
  EXPECT_GT(a, 100);
}

TEST(TimerTest, StartStopStartPeriodTimer)
{
  int a = 0;
  sf::Timer periodTimer;
  periodTimer.start(500, [&a]()
    {
      a += 100;
    });
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  periodTimer.stop();
  periodTimer.start(600, [&a]()
    {
      a += 50;
    });

  std::this_thread::sleep_for(std::chrono::seconds(2));
  periodTimer.stop();
  EXPECT_GT(a, 50);
}
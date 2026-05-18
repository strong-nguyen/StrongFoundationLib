#include "../src/ScopeStopwatch.h"

#include <gtest/gtest.h>


TEST(ScopeStopwatchTest, TestWithDefaultMillisecondPeriod)
{
  {
    sf::ScopeStopwatch stopwatch([](double elapsedTime)
      {
        std::cout << "This scope run in " << elapsedTime << " ms" << std::endl;
      });

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

TEST(ScopeStopwatchTest, TestWithSecondPeriod)
{
  auto lambda = []()
    {
      sf::ScopeStopwatch<std::ratio<1>> stopwatch([](double elapsedTime)
        {
          std::cout << "This scope run in " << elapsedTime << " second" << std::endl;
        });

      std::this_thread::sleep_for(std::chrono::seconds(2));
    };

  lambda();
}

TEST(ScopeStopwatchTest, TestWithMicrosecondPeriod)
{
  auto lambda = []()
    {
      sf::ScopeStopwatch<std::micro> stopwatch([](double elapsedTime)
        {
          std::cout << "This scope run in " << elapsedTime << " micro second" << std::endl;
        });

      std::this_thread::sleep_for(std::chrono::seconds(2));
    };

  lambda();
}
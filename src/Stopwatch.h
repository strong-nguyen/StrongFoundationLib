#pragma once


#include <chrono>

namespace sf
{
  class Stopwatch
  {
  public:
    Stopwatch();

    void start();

    bool stop();

    template <typename Period = std::ratio<1>>  // Return seconds in double by default
    double elapsedTime() const
    {
      return std::chrono::duration_cast<std::chrono::duration<double, Period>>(_elapsedTime).count();
    }

  private:
    std::chrono::steady_clock::time_point _startTime;
    std::chrono::steady_clock::duration _elapsedTime = {};
    bool _isStart = false;
  };
}

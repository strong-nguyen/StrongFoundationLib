#include "Stopwatch.h"


sf::Stopwatch::Stopwatch()
{
  start();
}

void sf::Stopwatch::start()
{
  _startTime = std::chrono::steady_clock::now();
  _isStart = true;
}

bool sf::Stopwatch::stop()
{
  if (!_isStart)
  {
    return false;
  }

  _elapsedTime = std::chrono::steady_clock::now() - _startTime;
  _isStart = false;
  return true;
}
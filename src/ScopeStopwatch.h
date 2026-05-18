#pragma once

#include "Stopwatch.h"

#include <functional>

namespace sf
{
  template <typename Period = std::milli>
  class ScopeStopwatch
  {
  public:
    ScopeStopwatch(std::function<void(double)> callback)
      :
      _callback(callback)
    {

    }

    ~ScopeStopwatch()
    {
      _stopwatch.stop();

      auto elapsedTime = _stopwatch.elapsedTime<Period>();
      if (_callback)
      {
        _callback(elapsedTime);
      }
    }

  private:
    Stopwatch _stopwatch;
    std::function<void(double)> _callback;
  };
}

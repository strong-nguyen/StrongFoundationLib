#include "Timer.h"

sf::Timer::Timer(TimerType timerType)
  :
  _timerType(timerType)
{
}

sf::Timer::~Timer()
{
  stop();
}

void sf::Timer::start(int intervalMs, std::function<void()> onTimeout)
{
  stop();

  {
    std::lock_guard lock(_mutex);
    _stop = false;
  }

  _worker = std::thread([this, intervalMs, onTimeout]()
    {
      while (true)
      {
        std::unique_lock lock(_mutex);
        _cv.wait_for(lock, std::chrono::milliseconds(intervalMs), [this]()
          {
            return _stop;
          });

        if (_stop)
        {
          break;
        }

        // Run task in a new detach thread
        if (onTimeout)
        {
          std::thread(onTimeout).detach();
        }

        if (_timerType == TimerType::OneTime)
        {
          _stop = true;
          break;
        }
      }
    });
}

void sf::Timer::stop()
{
  {
    std::lock_guard lock(_mutex);
    if (!_stop)
    {
      _stop = true;
    }
  }

  _cv.notify_all();
  if (_worker.joinable())
  {
    _worker.join();
  }
}

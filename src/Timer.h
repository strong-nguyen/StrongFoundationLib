#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>


namespace sf
{
  enum class TimerType
  {
    OneTime,
    Period
  };

  class Timer
  {
  public:
    Timer(TimerType timerType = TimerType::Period);  // By default, it is a period timer

    ~Timer();

    void start(int intervalMs, std::function<void()> onTimeout);

    void stop();

  private:
    TimerType _timerType;
    std::condition_variable _cv;
    std::mutex _mutex;
    bool _stop = false;
    std::thread _worker;
  };
}

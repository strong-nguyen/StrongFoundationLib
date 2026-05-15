#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <functional>


namespace sf
{
  class ThreadPool
  {
  public:
    ThreadPool(int poolSize = 8);

    ThreadPool(const ThreadPool& other) = delete;

    const ThreadPool& operator=(const ThreadPool& other) = delete;

    ~ThreadPool();

    template <typename Function, typename... Args>
    void addTask(Function func, Args... args)
    {
      // Package function and args into a lambda
      auto lambda = [func = std::forward<Function>(func), ...args = std::forward<Args>(args)]()
        {
          func(args...);
        };

      {
        std::lock_guard lock(_taskMutex);
        _tasks.push(lambda);
      }

      _taskNoti.notify_one();  // Notify a waiting thread
    }

    void stop(bool waitPendingTask = false);


  private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;  // FIFO queue of tasks to execute
    std::atomic<bool> _isStop = false;
    std::condition_variable _taskNoti;  // Notify when there is new task or client want to stop the pool
    std::mutex _taskMutex;  // Synchronize tasks between threads
  };
}

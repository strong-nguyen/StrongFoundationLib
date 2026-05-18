#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <future>


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
    auto addTask(Function func, Args... args) -> std::future<typename std::invoke_result<Function, Args...>::type>
    {
      // 1. Determine the return type of the function
      using ReturnType = typename std::invoke_result<Function, Args...>::type;

      auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
      std::future<ReturnType> result = task->get_future();
      auto lambda = [task]()
        {
          (*task)();
        };

      {
        std::lock_guard lock(_taskMutex);
        _tasks.push(lambda);
      }

      _taskNoti.notify_one();  // Notify a waiting thread

      return result;
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

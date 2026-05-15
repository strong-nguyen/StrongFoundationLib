#include "ThreadPool.h"



sf::ThreadPool::ThreadPool(int poolSize)
{
  for (int i = 0; i < poolSize; ++i)
  {
    _workers.push_back(std::thread([this]()
      {
        while (true)
        {
          std::unique_lock<std::mutex> lock(_taskMutex);
          _taskNoti.wait(lock, [this]()
            {
              return !_tasks.empty() || _isStop;
            });

          if (_isStop && _tasks.empty())  // While wating for tasks, the ThreadPool is stop and there is no tasks left then worker thread will exit
          {
            break;
          }

          if (_tasks.empty())  // Ensure there is task to take out of the queue
          {
            continue;
          }

          std::function<void()> task = std::move(_tasks.front());
          _tasks.pop();
          lock.unlock();

          if (task)
          {
            task();
          }
        }
      }));
  }
}

sf::ThreadPool::~ThreadPool()
{
  stop(true);  // Waiting for pending tasks
}

void sf::ThreadPool::stop(bool waitPendingTask /* = false */)
{
  if (_isStop.exchange(true))  // The pool already stopped before
  {
    return;
  }

  if (!waitPendingTask)
  {
    std::lock_guard lock(_taskMutex);
    _tasks = {};
  }
  
  _taskNoti.notify_all();
  for (auto& worker : _workers)
  {
    worker.join();
  }
}

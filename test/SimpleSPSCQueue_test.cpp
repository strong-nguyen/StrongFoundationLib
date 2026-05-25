#include <gtest/gtest.h>
#include "../src/lock_free/SimpleSPSCQueue.h"
#include <thread>
#include <string>
#include <format>

TEST(SimpleSPSCTest, EnqueueAndDequeueMultithread)
{
  sf::SimpleSPSCQueue<std::string> queue;

  std::thread t1([&queue]()
                 {
    for (int i = 0; i < 10000; ++i)
    {
      queue.enqueue(std::format("Message {}", i));
    } });

  std::thread t2([&queue]()
                 {
    for (int i = 0; i < 10000; ++i)
    {
      auto result = queue.dequeue();
      if (result)
      {
        // std::cout << *result << std::endl;
      }
      else
      {
        std::cout << "Queue is empty, failed to dequeue" << std::endl;
      }
    } });

  t1.join();
  t2.join();
};
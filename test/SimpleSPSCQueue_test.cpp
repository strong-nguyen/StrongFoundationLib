#include "../src/lock_free/SimpleSPSCQueue.h"
#include <format>
#include <gtest/gtest.h>
#include <string>
#include <thread>

TEST(SimpleSPSCTest, EnqueueAndDequeueSameThread) {
  sf::SimpleSPSCQueue<std::string> queue;

  auto front = queue.dequeue();
  EXPECT_EQ(front.has_value(), false);

  queue.enqueue("Message 1");
  front = queue.dequeue();
  EXPECT_EQ(front.has_value(), true);
  EXPECT_EQ(front.value(), "Message 1");
};

TEST(SimpleSPSCTest, EnqueueAndDequeueMultiThread) {
  sf::SimpleSPSCQueue<std::string> queue;

  std::thread t1([&queue]() {
    for (size_t i = 0; i < 1'000'000; ++i) {
      queue.enqueue(std::format("Message {}", i));
    }
  });

  std::thread t2([&queue]() {
    for (size_t i = 0; i < 1'000'000; ++i) {
      queue.dequeue();
    }
  });

  t1.join();
  t2.join();

  if (!queue.empty()) {
    auto front = queue.dequeue();
    EXPECT_EQ(front.has_value(), true);
  }
}

TEST(SimpleSPSCTest, CheckQueueEmpty) {
  sf::SimpleSPSCQueue<std::string> queue;
  EXPECT_EQ(queue.empty(), true);

  queue.enqueue("Message 1");
  EXPECT_EQ(queue.empty(), false);

  auto front = queue.dequeue();
  EXPECT_EQ(front.has_value(), true);

  EXPECT_EQ(front.value(), "Message 1");
}
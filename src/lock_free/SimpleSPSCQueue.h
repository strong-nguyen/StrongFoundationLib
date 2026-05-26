#pragma once

#include <atomic>
#include <optional>

namespace sf
{
  template <typename T>
  class SimpleSPSCQueue
  {
  private:
    struct Node
    {
      T data{};
      std::atomic<Node *> next = nullptr;

      Node() = default;

      Node(const T &val)
          : data{val}
      {
      }
    };

    std::atomic<Node *> _head; // Head point to the front of the queue => pop will take the _head
    std::atomic<Node *> _tail; // Tail point to the back of the queue => push will append to _tail

  public:
    SimpleSPSCQueue()
    {
      // Start with dummy node
      Node *dummy = new Node();
      _head.store(dummy);
      _tail.store(dummy);
    }

    void enqueue(const T &value)
    {
      Node *newNode = new Node(value);
      Node *oldTail = _tail.load();
      oldTail->next.store(newNode);
      _tail.store(newNode);
    }

    std::optional<T> dequeue()
    {
      Node *oldHead = _head.load();
      Node *nextNode = oldHead->next.load();

      // If queue is empty
      if (nextNode == nullptr)
      {
        return {};
      }

      T result = nextNode->data;
      _head.store(nextNode);
      delete oldHead;
      return result;
    }

    bool empty() const
    {
      // We use acquire memory order here to make sure we see the newest nodes pushed by the producer thread.
      Node *current_head = _head.load(std::memory_order_acquire);
      return current_head->next.load(std::memory_order_acquire) == nullptr;
    }
  };
}

#pragma once


template <typename T>
class FixedSizeMemoryPool
{
  struct Node
  {
    Node* next = nullptr;  // Pointer to next node
  };
public:
  FixedSizeMemoryPool(size_t capacity)
    :
    _capacity(capacity),
    _freeListHead(nullptr)
  {
    _rawBuffer = new char[_capacity * BlockSize];

    for (size_t i = 0; i < _capacity; ++i)
    {
      Node* currentNode = reinterpret_cast<Node*>(_rawBuffer + i * BlockSize);

      // The node will be insert in backward order: Node N -> ... -> Node 3 -> Node 2 -> Head
      currentNode->next = _freeListHead;
      _freeListHead = currentNode;
    }
  }

  ~FixedSizeMemoryPool()
  {
    delete[] _rawBuffer;
  }

  void* allocate()
  {
    if (_freeListHead)
    {
      Node* availableNode = _freeListHead;
      _freeListHead = _freeListHead->next;
      return availableNode;
    }

    // Out of memory
    return nullptr;
  }

  void deallocate(void* object)
  {
    if (!object)
    {
      return;
    }

    Node* node = reinterpret_cast<Node*>(object);
    node->next = _freeListHead;
    _freeListHead = node;
  }

  size_t countPoolFree() const
  {
    if (!_freeListHead)
    {
      return 0;
    }

    size_t count = 0;
    Node* head = _freeListHead;
    while (head)
    {
      ++count;
      head = head->next;
    }
    return count;
  }

private:
  size_t _capacity;
  char* _rawBuffer;
  Node* _freeListHead;

  // If object size is smaller than Node size, use Node size as slot size, else use object size
  static constexpr size_t BlockSize = sizeof(T) < sizeof(Node) ? sizeof(Node) : sizeof(T);
};
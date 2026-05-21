#include "MemoryPool.h"


sf::FixedSizeBytePool::FixedSizeBytePool(size_t capacity, size_t objectSize)
  :
  _capacity(capacity),
  _freeListHead(nullptr)
{
  const size_t blockSize = std::max(objectSize, sizeof(Node));
  _rawBuffer = new char[_capacity * blockSize];

  for (size_t i = 0; i < _capacity; ++i)
  {
    Node* currentNode = reinterpret_cast<Node*>(_rawBuffer + i * blockSize);

    // The node will be insert in backward order: Node N -> ... -> Node 3 -> Node 2 -> Head
    currentNode->next = _freeListHead;
    _freeListHead = currentNode;
  }
}

sf::FixedSizeBytePool::~FixedSizeBytePool()
{
  delete[] _rawBuffer;
}

void* sf::FixedSizeBytePool::allocate()
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

void sf::FixedSizeBytePool::deallocate(void* object)
{
  if (!object)
  {
    return;
  }

  Node* node = reinterpret_cast<Node*>(object);
  node->next = _freeListHead;
  _freeListHead = node;
}

size_t sf::FixedSizeBytePool::countFree() const
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
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

sf::MultiMemoryPool::MultiMemoryPool(size_t capacity)
  :
  _pools
  {
    FixedSizeBytePool(capacity, 32),
    FixedSizeBytePool(capacity, 64),
    FixedSizeBytePool(capacity, 128),
    FixedSizeBytePool(capacity, 256)
  }
{
}

void* sf::MultiMemoryPool::allocate(size_t bytes)
{
  int poolIndex = getPoolIndex(bytes);
  if (poolIndex != -1)
  {
    return _pools[poolIndex].allocate();
  }

  // Fallback to default heap allocation
  return ::operator new(bytes);
}

void sf::MultiMemoryPool::deallocate(void* object, size_t bytes)
{
  if (!object)
  {
    return;
  }

  int poolIndex = getPoolIndex(bytes);
  if (poolIndex != -1)
  {
    _pools[poolIndex].deallocate(object);
  }
  else
  {
    ::operator delete(object);
  }
}

int sf::MultiMemoryPool::getPoolIndex(size_t bytes) const
{
  if (bytes <= 32)
  {
    return 0;
  }
  if (bytes <= 64)
  {
    return 1;
  }
  if (bytes <= 128)
  {
    return 2;
  }
  if (bytes <= 256)
  {
    return 3;
  }

  return -1;
}
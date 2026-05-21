#pragma once

#include <algorithm>

namespace sf
{
  class FixedSizeBytePool
  {
    struct Node
    {
      Node* next = nullptr;  // Pointer to next node
    };
  public:
    FixedSizeBytePool(size_t capacity, size_t objectSize);

    FixedSizeBytePool(const FixedSizeBytePool& other) = delete;

    virtual ~FixedSizeBytePool();

    void* allocate();

    void deallocate(void* object);

    size_t countFree() const;

  private:
    size_t _capacity;
    char* _rawBuffer;
    Node* _freeListHead;
  };


  /*
  * For the performance of MemoryPool, FixedSizeMemoryPool is not designed for thread-safe!
  * This is the template version for FixedSizeMemoryPool
  */
  template <typename T>
  class FixedSizeMemoryPool : public FixedSizeBytePool
  {
    struct Node
    {
      Node* next = nullptr;  // Pointer to next node
    };
  public:
    FixedSizeMemoryPool(size_t capacity)
      :
      FixedSizeBytePool(capacity, sizeof(T))
    {
    }

    ~FixedSizeMemoryPool() = default;

    T* allocateType()
    {
      void* mem = allocate();
      if (!mem)
      {
        return nullptr;
      }
      else
      {
        return reinterpret_cast<T*>(mem);
      }
    }

    template <typename... Args>
    T* create(Args&&... args) {
      void* ptr = this->allocate();
      if (!ptr) return nullptr;

      // Construct the object directly in the allocated pool slot
      return ::new (ptr) T(std::forward<Args>(args)...);
    }
  };


  /*
  * Multi MemoryPool contains a 
  */
  class MultiMemoryPool
  {
  public:
    MultiMemoryPool(size_t capacity);

    void* allocate(size_t bytes);

    void deallocate(void* object, size_t bytes);

    template <typename T, typename... Args>
    T* create(Args&&... args)
    {
      void* ptr = this->allocate(sizeof(T));
      if (!ptr) return nullptr;
      return ::new (ptr) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void destroy(T* object)
    {
      if (!object) return;
      object->~T();
      this->deallocate(object, sizeof(T));
    }

  private:
    int getPoolIndex(size_t bytes) const;

    FixedSizeBytePool _pools[4];  // Contains 4 memory pool with size 32,64,128,256
  };
}

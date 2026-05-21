#pragma once

#include "../src/MemoryPool.h"

#include <gtest/gtest.h>


struct Point2D
{
  double x = 0.0;
  double y = 0.0;
};


TEST(FixedMemoryPoolTest, Pool1ElementAllocateThenDeallocate)
{
  sf::FixedSizeMemoryPool<Point2D> mp(1);

  auto mem1 = mp.allocate();
  Point2D* point1 = new (mem1) Point2D(10.5, 9.5);
  EXPECT_NE(mem1, nullptr);

  auto mem2 = mp.allocate();
  EXPECT_EQ(mem2, nullptr);

  mp.deallocate(point1);
}

TEST(FixedMemoryPoolTest, Pool1ElementAllocateThenDeallocateThenAllocate)
{
  sf::FixedSizeMemoryPool<Point2D> mp(1);

  auto mem1 = mp.allocate();
  Point2D* point1 = new (mem1) Point2D(10.5, 9.5);
  EXPECT_NE(mem1, nullptr);

  auto mem2 = mp.allocate();
  EXPECT_EQ(mem2, nullptr);

  mp.deallocate(point1);
  mem1 = mp.allocate();
  EXPECT_NE(mem1, nullptr);
}

TEST(FixedMemoryPoolTest, Pool10ElementAllocateThenDeallocate)
{
  sf::FixedSizeMemoryPool<Point2D> mp(10);
  std::vector<Point2D*> points;

  for (int i = 0; i < 10; ++i)
  {
    auto mem = mp.allocate();
    EXPECT_NE(mem, nullptr);
    points.push_back(new (mem) Point2D(i, i));
  }

  EXPECT_EQ(points.size(), 10);
  for (int i = 0; i < 10; ++i)
  {
    EXPECT_EQ(points[i]->x, i);
    EXPECT_EQ(points[i]->y, i);
  }

  for (int i = 0; i < 10; ++i)
  {
    mp.deallocate(points[i]);
  }
}


struct Point3D
{
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  static sf::FixedSizeMemoryPool<Point3D> mp;

  void* operator new(size_t size)
  {
    return mp.allocate();
  }

  void operator delete(void* ptr)
  {
    mp.deallocate(ptr);
  }
};

sf::FixedSizeMemoryPool<Point3D> Point3D::mp{ 10 };

TEST(FixedMemoryPoolTest, OverloadNewAndDeleteOperatorWithMemoryPool)
{
  std::vector<Point3D*> points;
  for (int i = 0; i < 10; ++i)
  {
    points.push_back(new Point3D(i, i , i));
  }

  EXPECT_EQ(Point3D::mp.countFree(), 0);
  for (int i = 0; i < 10; ++i)
  {
    delete points[i];
    points[i] = nullptr;
  }

  EXPECT_EQ(Point3D::mp.countFree(), 10);
}

TEST(FixedMemoryPoolTest, AllocateType)
{
  sf::FixedSizeMemoryPool<Point2D> mp(10);
  auto mem = mp.allocateType();
  ASSERT_NE(mem, nullptr);
  EXPECT_EQ(typeid(Point2D*), typeid(mem));

  auto point1 = new (mem) Point2D(1.0, 2.0);
  EXPECT_EQ(point1->x, 1.0);
  EXPECT_EQ(point1->y, 2.0);

  mp.deallocate(point1);
}

TEST(FixedMemoryPoolTest, CreateObject)
{
  sf::FixedSizeMemoryPool<Point2D> mp(10);
  auto point1 = mp.create(1.0, 2.0);
  ASSERT_NE(point1, nullptr);
  EXPECT_EQ(point1->x, 1.0);
  EXPECT_EQ(point1->y, 2.0);

  mp.deallocate(point1);
}

TEST(TestFixedSizeBytePool, AllocateThenDeallocateOject)
{
  sf::FixedSizeBytePool mp(1, sizeof(Point2D));
  auto mem = mp.allocate();
  ASSERT_NE(mem, nullptr);

  Point2D* point = new (mem) Point2D(1.0, 2.0);
  EXPECT_EQ(point->x, 1.0);
  EXPECT_EQ(point->y, 2.0);
  EXPECT_EQ(mp.countFree(), 0);
  mp.deallocate(point);
  EXPECT_EQ(mp.countFree(), 1);
}

TEST(TestFixedSizeBytePool, Allocate1ThenDeallocateOject)
{
  sf::FixedSizeBytePool mp(1, sizeof(Point2D));
  auto mem = mp.allocate();
  ASSERT_NE(mem, nullptr);

  Point2D* point = new (mem) Point2D(1.0, 2.0);
  EXPECT_EQ(point->x, 1.0);
  EXPECT_EQ(point->y, 2.0);
  EXPECT_EQ(mp.countFree(), 0);
  mp.deallocate(point);
  EXPECT_EQ(mp.countFree(), 1);
}

TEST(TestFixedSizeBytePool, Allocate10ThenDeallocateOject)
{
  sf::FixedSizeBytePool mp(10, sizeof(Point2D));
  std::vector<Point2D*> points;
  points.reserve(10);
  for (int i = 0; i < 10; ++i)
  {
    auto mem = mp.allocate();
    ASSERT_NE(mem, nullptr);

    Point2D* point = new (mem) Point2D(i, i);
    points.push_back(point);
  }

  EXPECT_EQ(mp.countFree(), 0);
  auto mem = mp.allocate();
  EXPECT_EQ(mem, nullptr);
  for (int i = 0; i < 10; ++i)
  {
    EXPECT_EQ(points[i]->x, (double)i);
    EXPECT_EQ(points[i]->y, (double)i);
    mp.deallocate(points[i]);
  }

  EXPECT_EQ(mp.countFree(), 10);
}


struct Bytes8
{
  double val = 0.0;
};

struct Bytes16
{
  Bytes8 val1;
  Bytes8 val2;

  Bytes16(const Bytes8& val1_, const Bytes8& val2_)
    :
    val1{val1_},
    val2{val2_}
  {
  }
};

struct Bytes32
{
  Bytes16 val1;
  Bytes16 val2;

  Bytes32(const Bytes16& val1_, const Bytes16& val2_)
    :
    val1{val1_},
    val2{val2_}
  {

  }
};

struct Bytes64
{
  Bytes32 val1;
  Bytes32 val2;
};

struct Bytes128
{
  Bytes64 val1;
  Bytes64 val2;
};

struct Bytes256
{
  Bytes128 val1;
  Bytes128 val2;
};


TEST(MultiMemoryPoolTest, MultiPool1AllocateThenDeallocate)
{
  sf::MultiMemoryPool mp(1);
  auto mem = mp.allocate(sizeof(Bytes8));
  EXPECT_NE(mem, nullptr);

  Bytes8* object = new (mem) Bytes8{ 1.0 };
  EXPECT_EQ(object->val, 1.0);
  mem = mp.allocate(sizeof(Bytes8));
  EXPECT_EQ(mem, nullptr);

  mp.deallocate(object, sizeof(Bytes8));
}

TEST(MultiMemoryPoolTest, MultiPool10AllocateThenDeallocate)
{
  sf::MultiMemoryPool mp(10);

  std::vector<std::pair<void*, int>> mems;
  for (int bytes = 8; bytes <= 256; bytes += 8)
  {
    mems.push_back({ mp.allocate(bytes), bytes });
  }

  EXPECT_TRUE(!mems.empty());
  for (const auto& e : mems)
  {
    if (e.first == nullptr)
    {
      continue;
    }

    if (e.second == 8)
    {
      Bytes8* mem = new (e.first) Bytes8{ 10.0 };
      EXPECT_EQ(mem->val, 10.0);
    }
    else if (e.second == 16)
    {
      Bytes16* mem = new (e.first) Bytes16({ 10.0 }, { 20.0 });
      EXPECT_EQ(mem->val1.val, 10.0);
      EXPECT_EQ(mem->val2.val, 20.0);
    }
    else if (e.second == 32)
    {
      Bytes32* mem = reinterpret_cast<Bytes32*>(e.first);
    }
    else if (e.second == 64)
    {
      Bytes64* mem = reinterpret_cast<Bytes64*>(e.first);
    }
    else if (e.second == 128)
    {
      Bytes128* mem = reinterpret_cast<Bytes128*>(e.first);
    }
    else if (e.second == 256)
    {
      Bytes256* mem = reinterpret_cast<Bytes256*>(e.first);
    }
  }
}
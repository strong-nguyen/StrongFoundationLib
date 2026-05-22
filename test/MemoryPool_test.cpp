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
  double val[1] = { 0.0 };

  Bytes8(double number)
  {
    val[0] = number;
  }

  bool operator==(const Bytes8& other) const
  {
    return val[0] == other.val[0];
  }
};

struct Bytes16
{
  double val[2] = { 0.0};

  Bytes16(std::initializer_list<double> arr)
  {
    size_t i = 0;
    for (double number : arr)
    {
      val[i++] = number;
    }
  }

  bool operator==(const Bytes16& other) const
  {
    std::vector<double> mine(val, val + 2);
    std::vector<double> otherVal(other.val, other.val + 2);
    return mine == otherVal;
  }
};

struct Bytes32
{
  double val[4] = { 0.0 };

  Bytes32(std::initializer_list<double> arr)
  {
    size_t i = 0;
    for (double number : arr)
    {
      val[i++] = number;
    }
  }

  bool operator==(const Bytes32& other) const
  {
    std::vector<double> mine(val, val + 4);
    std::vector<double> otherVal(other.val, other.val + 4);
    return mine == otherVal;
  }
};

struct Bytes64
{
  double val[8] = { 0.0 };

  Bytes64(std::initializer_list<double> arr)
  {
    size_t i = 0;
    for (double number : arr)
    {
      val[i++] = number;
    }
  }

  bool operator==(const Bytes64& other) const
  {
    std::vector<double> mine(val, val + 8);
    std::vector<double> otherVal(other.val, other.val + 8);
    return mine == otherVal;
  }
};

struct Bytes128
{
  double val[16] = { 0.0 };

  Bytes128(std::initializer_list<double> arr)
  {
    size_t i = 0;
    for (double number : arr)
    {
      val[i++] = number;
    }
  }

  bool operator==(const Bytes128& other) const
  {
    std::vector<double> mine(val, val + 16);
    std::vector<double> otherVal(other.val, other.val + 16);
    return mine == otherVal;
  }
};

struct Bytes256
{
  double val[32] = { 0.0 };

  Bytes256(std::initializer_list<double> arr)
  {
    size_t i = 0;
    for (double number : arr)
    {
      val[i++] = number;
    }
  }

  bool operator==(const Bytes256& other) const
  {
    std::vector<double> mine(val, val + 32);
    std::vector<double> otherVal(other.val, other.val + 32);
    return mine == otherVal;
  }
};


TEST(MultiMemoryPoolTest, MultiPool1AllocateThenDeallocate)
{
  sf::MultiMemoryPool mp(1);
  auto mem = mp.allocate(sizeof(Bytes8));
  EXPECT_NE(mem, nullptr);

  Bytes8* object = new (mem) Bytes8{ 1.0 };
  EXPECT_EQ(object->val[0], 1.0);
  mem = mp.allocate(sizeof(Bytes8));
  EXPECT_EQ(mem, nullptr);

  mp.deallocate(object, sizeof(Bytes8));
}

TEST(MultiMemoryPoolTest, MultiPool10AllocateThenDeallocate)
{
  sf::MultiMemoryPool mp(100);

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
      EXPECT_EQ(mem->val[0], 10.0);
    }
    else if (e.second == 16)
    {
      Bytes16* mem = new (e.first) Bytes16({ 1.0, 2.0 });
      EXPECT_EQ((*mem), (Bytes16({ 1.0, 2.0 })));
    }
    else if (e.second == 32)
    {
      Bytes32* mem = new (e.first) Bytes32({10.0, 20.0, 30.0, 40.0});
      EXPECT_EQ((*mem), (Bytes32({ 10.0, 20.0, 30.0, 40.0 })));
    }
    else if (e.second == 64)
    {
      Bytes64* mem = new (e.first) Bytes64({ 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0 });
      EXPECT_EQ((*mem), (Bytes64({ 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0 })));
    }
    else if (e.second == 128)
    {
      Bytes128* mem = new (e.first) Bytes128({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
      EXPECT_EQ((*mem), (Bytes128({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 })));
    }
    else if (e.second == 256)
    {
      Bytes256* mem = new (e.first) Bytes256({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 ,26, 27, 28, 29, 30 , 31, 32});
      EXPECT_EQ((*mem), (Bytes256({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 ,26, 27, 28, 29, 30 , 31, 32 })));
    }
  }
}

TEST(MultiMemoryPoolTest, MultiPool10CreateThenDestroy)
{
  sf::MultiMemoryPool mp(10);
  auto byte8 = mp.create<Bytes8>(1.0);
  ASSERT_NE(byte8, nullptr);
  EXPECT_EQ(typeid(byte8), typeid(Bytes8*));
  EXPECT_EQ(byte8->val[0], 1.0);

  auto byte16 = mp.create<Bytes16>(std::initializer_list<double>{ 1.0, 2.0 });
  ASSERT_NE(byte16, nullptr);
  EXPECT_EQ(typeid(byte16), typeid(Bytes16*));
  EXPECT_EQ(*byte16, (Bytes16{1.0, 2.0}));

  mp.destroy(byte8);
  mp.destroy(byte16);
}
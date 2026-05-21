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
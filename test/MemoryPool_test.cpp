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

TEST(FixedMemoryPoolTest, AllocateDifferentTypeOfObject)
{
  
}
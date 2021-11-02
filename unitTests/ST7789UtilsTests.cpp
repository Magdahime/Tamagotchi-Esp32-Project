#include "ST7789Utils.hpp"
#include "gtest/gtest.h"
using namespace tamagotchi::ST7789;

TEST(ST7789Utils, sort3PointsTest) {
  Point point1{0, 2};
  Point point2{1, 1};
  Point point3{2, 0};
  auto [a, b, c] = sort3Points(point2, point3, point1, Coordinate::X);
  EXPECT_EQ(a.x, point3.x);
  EXPECT_EQ(a.y, point3.y);
  EXPECT_EQ(b.x, point2.y);
  EXPECT_EQ(b.y, point2.y);
  EXPECT_EQ(c.x, point1.x);
  EXPECT_EQ(c.y, point1.y);
  auto [d, e, f] = sort3Points(point2, point3, point1, Coordinate::Y);
  EXPECT_EQ(d.x, point1.x);
  EXPECT_EQ(d.y, point1.y);
  EXPECT_EQ(e.x, point2.x);
  EXPECT_EQ(e.y, point2.y);
  EXPECT_EQ(f.x, point3.x);
  EXPECT_EQ(f.y, point3.y);
}

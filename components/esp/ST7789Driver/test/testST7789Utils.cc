#include "ST7789Utils.hpp"

#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::ST7789;



TEST_GROUP(ST7789VWUtilsTests);

TEST_SETUP(ST7789VWUtilsTests) {}

TEST_TEAR_DOWN(ST7789VWUtilsTests) {
}


TEST(ST7789VWUtilsTests, sort3PointsTest) {
  Point point1{0, 2};
  Point point2{1, 1};
  Point point3{2, 0};
  auto [a, b, c] = sort3Points(point2, point3, point1, Coordinate::X);
  TEST_ASSERT_EQUAL_INT(a.x_, point3.x_);
  TEST_ASSERT_EQUAL_INT(a.y_, point3.y_);
  TEST_ASSERT_EQUAL_INT(b.x_, point2.y_);
  TEST_ASSERT_EQUAL_INT(b.y_, point2.y_);
  TEST_ASSERT_EQUAL_INT(c.x_, point1.x_);
  TEST_ASSERT_EQUAL_INT(c.y_, point1.y_);
  auto [d, e, f] = sort3Points(point2, point3, point1, Coordinate::Y);
  TEST_ASSERT_EQUAL_INT(d.x_, point1.x_);
  TEST_ASSERT_EQUAL_INT(d.y_, point1.y_);
  TEST_ASSERT_EQUAL_INT(e.x_, point2.x_);
  TEST_ASSERT_EQUAL_INT(e.y_, point2.y_);
  TEST_ASSERT_EQUAL_INT(f.x_, point3.x_);
  TEST_ASSERT_EQUAL_INT(f.y_, point3.y_);
}

TEST_GROUP_RUNNER(ST7789VWUtilsTests) {
  RUN_TEST_CASE(ST7789VWUtilsTests, sort3PointsTest)
}
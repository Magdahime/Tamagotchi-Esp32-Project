#include "EspGLUtils.hpp"
#include "EspGLVect2.hpp"
#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::EspGL;

TEST_GROUP(EspGLUtilsTests);

TEST_SETUP(EspGLUtilsTests) {}

TEST_TEAR_DOWN(EspGLUtilsTests) {}

TEST(EspGLUtilsTests, sort3PointsTest) {
  Vect2 point1{0, 2};
  Vect2 point2{1, 1};
  Vect2 point3{2, 0};
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

TEST(EspGLUtilsTests, Vect2AddingTwoVectors) {
  Vect2 point1{0, 2};
  Vect2 point2{1, 1};
  Vect2 point3 = point1 + point2;
  TEST_ASSERT_EQUAL_INT(point3.x_, 1);
  TEST_ASSERT_EQUAL_INT(point3.y_, 3);
}

TEST(EspGLUtilsTests, Vect2SubtractingTwoVectors) {
  Vect2 point1{0, 2};
  Vect2 point2{1, 1};
  Vect2 point3 = point1 - point2;
  TEST_ASSERT_EQUAL_INT(point3.x_, -1);
  TEST_ASSERT_EQUAL_INT(point3.y_, 1);
}

TEST(EspGLUtilsTests, Vect2AddingConstant) {
  Vect2 point1{0, 2};
  Vect2 point2 = point1 + 1;
  TEST_ASSERT_EQUAL_INT(point2.x_, 1);
  TEST_ASSERT_EQUAL_INT(point2.y_, 3);
}

TEST(EspGLUtilsTests, Vect2SubtractingConstant) {
  Vect2 point1{0, 2};
  Vect2 point2 = point1 - 1;
  TEST_ASSERT_EQUAL_INT(point2.x_, -1);
  TEST_ASSERT_EQUAL_INT(point2.y_, 1);
}

TEST_GROUP_RUNNER(EspGLUtilsTests) {
  RUN_TEST_CASE(EspGLUtilsTests, sort3PointsTest)
  RUN_TEST_CASE(EspGLUtilsTests, Vect2AddingTwoVectors)
  RUN_TEST_CASE(EspGLUtilsTests, Vect2SubtractingTwoVectors)
  RUN_TEST_CASE(EspGLUtilsTests, Vect2AddingConstant)
  RUN_TEST_CASE(EspGLUtilsTests, Vect2SubtractingConstant)
}
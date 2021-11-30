
#include "testST7789Driver.hpp"

#include "unity.h"
#include "unity_fixture.h"

#include "ST7789Conf.hpp"

using namespace tamagotchi;

ST7789::ST7789VWDriver driver = initializeDisplay();

TEST_GROUP(ST7789VWDriverTests);

TEST_SETUP(ST7789VWDriverTests) {}

TEST_TEAR_DOWN(ST7789VWDriverTests) {
  driver.delay(1000);
  driver.fillScreen(ST7789::colours::BLACK);
  driver.delay(1000);
}

TEST(ST7789VWDriverTests, FillingDisplayTest) {
  driver.fillScreen(ST7789::colours::RED);
}

TEST(ST7789VWDriverTests, DrawingPixelTest) {
  driver.drawPixel({150, 150}, ST7789::colours::RED);
}

TEST(ST7789VWDriverTests, DrawingPixelLine) {
  driver.drawPixelLine({150, 150}, 30, ST7789::colours::RED);
}

TEST(ST7789VWDriverTests, DrawingFilledRectangle) {
  driver.drawFilledRectangle({50, 50}, {120, 120}, ST7789::colours::RED);
}

TEST(ST7789VWDriverTests, DrawingFilledRectangles) {
  driver.drawFilledRectangle({50, 50}, {120, 120}, ST7789::colours::RED);
  driver.drawFilledRectangle({239, 239}, {80, 80}, ST7789::colours::BLUE);
}

TEST(ST7789VWDriverTests, DrawingHorizontalLine) {
  driver.drawLine({50, 50}, {50, 120}, ST7789::colours::RED);
}

TEST(ST7789VWDriverTests, DrawingVerticalLine) {
  driver.drawLine({50, 50}, {120, 50}, ST7789::colours::RED);
}

TEST(ST7789VWDriverTests, DrawingLines) {
  ST7789::Point center{100, 100};
  int arms = 30;
  for (int i = 0; i < arms; i++) {
    float angle = 2 * tamagotchi::ST7789::consts::PI / arms * i;
    driver.drawLine(center,
                    {center.x_ + sin(angle) * 50, center.y_ + cos(angle) * 50},
                    ST7789::colours::BLUE);
  }
}

TEST(ST7789VWDriverTests, DrawingRectangleOutline) {
  driver.drawRectangle({50, 50}, {120, 120}, ST7789::colours::RED);
  driver.drawRectangle({239, 239}, {80, 80}, ST7789::colours::BLUE);
}

TEST(ST7789VWDriverTests, DrawingTriangle) {
  driver.drawTriangle({50, 50}, {120, 120}, {239, 40}, ST7789::colours::BLUE);
}

TEST(ST7789VWDriverTests, DrawingFilledTriangle) {
  driver.drawFilledTriangle({50, 50}, {120, 120}, {239, 40},
                            ST7789::colours::BLUE);
}

TEST(ST7789VWDriverTests, DrawingCircle) {
  driver.drawCircle({100, 100}, 50, ST7789::colours::YELLOW);
}

TEST(ST7789VWDriverTests, DrawingFilledCircle) {
  driver.drawFilledCircle({100, 100}, 50, ST7789::colours::YELLOW);
}

TEST(ST7789VWDriverTests, DrawingPolygon) {
  driver.drawPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW);
}

TEST(ST7789VWDriverTests, DrawingRotatedPolygon) {
  driver.drawPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW,
                     ST7789::consts::PI / 4.0);
}

TEST(ST7789VWDriverTests, DrawingRotatedPolygonWithManyVertices) {
  driver.drawPolygon({100, 100}, 50, 16, ST7789::colours::YELLOW,
                     ST7789::consts::PI / 4.0);
}

TEST(ST7789VWDriverTests, DrawingFilledPolygon) {
  driver.drawFilledPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW);
}

TEST(ST7789VWDriverTests, DrawingFilledRotatedPolygon) {
  driver.drawFilledPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW,
                           ST7789::consts::PI / 4.0);
}

TEST(ST7789VWDriverTests, DrawingFilledRotatedPolygonWithManyVertices) {
  driver.drawFilledPolygon({100, 100}, 50, 16, ST7789::colours::YELLOW,
                           ST7789::consts::PI / 4.0);
}

TEST(ST7789VWDriverTests, DrawingBitmap) {
  std::vector<bool> test{
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  false, false, false, false,
      false, false, true,  false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  false, false, false, false,
      false, false, true,  false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  false, false, false, false,
      false, false, true,  false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  false, false, false, false,
      false, false, true,  false, false, false, false, false, false, false,
      false, false, false, false, false, false, true,  false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, true,  false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, true,  false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, true,  false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, true,  false, false, false, false, false, false,
      false, false, false, false, false, false, true,  true,  false, false,
      false, false, false, false, false, false, false, false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, true,  false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, true,  false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, true,  true,  false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  true,  false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, true,  true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, true,  true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      true,  true,  true,  false, false, false, false, false, false, false,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, true,  true,  true,  true,  true,  true,  true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  true,  true,  false, false,
      false, true,  true,  true,  true,  false, false, false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, true,  false,
      false, true,  false, false, false, true,  false, false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, true,  false, false, false, false, false, true,
      false, true,  false, false, false, false, true,  false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, true,  false, false, false, false, false, false,
      false, true,  false, false, false, false, false, true,  false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, true,  true,  false, false, false, false,
      false, true,  false, false, false, false, false, true,  false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, true,  false, false, false,
      false, true,  false, false, false, false, false, true,  false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, true,  true,  false,
      false, true,  false, false, false, false, false, true,  false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, true,  false,
      false, true,  false, false, false, false, false, true,  false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, true,  false, false, false, false, false, true,  false,
      false, true,  false, false, false, false, true,  false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, true,  false, false, true,  true,  false, false,
      false, true,  false, false, false, true,  false, false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, true,  false, true,  false, false, false,
      false, true,  true,  true,  true,  false, false, false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, true,  true,  true,  true,  false, false, false, false,
      false, true,  true,  true,  true,  true,  false, false, false, false,
      false, true,  true,  true,  true,  false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, true,  false, false, false, false, true,  false, false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      true,  false, false, false, false, true,  false, false, false, false,
      false, false, false, true,  true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      true,  false, false, false, false, false, false, true,  false, false,
      true,  false, false, false, false, false, true,  false, false, true,
      false, false, false, false, false, false, true,  false, false, false,
      false, false, true,  false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, true,  false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      false, false, false, false, false, false, true,  false, false, false,
      false, true,  false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, true,  false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      false, false, false, false, false, false, true,  false, false, false,
      true,  false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, true,  false, false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      false, false, false, false, false, true,  false, false, false, true,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, true,  false, false, false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      false, false, false, true,  false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, true,  false, false, false, false, false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      false, false, true,  false, false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, true,  false, false, false, false, false, false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      false, true,  false, false, false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, true,  false, false, false, false, false, false, false, false,
      true,  false, false, false, false, false, true,  false, false, false,
      true,  false, false, false, false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      true,  false, false, false, false, false, false, false, false, false,
      true,  false, false, false, false, false, true,  false, false, true,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  false,
      false, true,  true,  true,  true,  true,  false, false, true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  false, false,
      false, false, false, false, true,  false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,

  };
  driver.drawBitmap({100, 100}, 50, 50, ST7789::colours::RED, test);
}

TEST_GROUP_RUNNER(ST7789VWDriverTests) {
  RUN_TEST_CASE(ST7789VWDriverTests, FillingDisplayTest)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingPixelTest)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingPixelLine)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledRectangle)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledRectangles)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingHorizontalLine)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingVerticalLine)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingLines)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingRectangleOutline)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingTriangle)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledTriangle)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingCircle)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledCircle)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingPolygon)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingRotatedPolygon)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingRotatedPolygonWithManyVertices)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledPolygon)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledRotatedPolygon)
  RUN_TEST_CASE(ST7789VWDriverTests,
                DrawingFilledRotatedPolygonWithManyVertices)
  RUN_TEST_CASE(ST7789VWDriverTests, DrawingBitmap)
}
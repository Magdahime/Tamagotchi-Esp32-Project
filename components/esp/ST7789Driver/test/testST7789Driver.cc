#include "testST7789Driver.hpp"

#include <unity.h>

#include "ST7789Conf.hpp"

using namespace tamagotchi;

ST7789::ST7789VWDriver driver = initializeDisplay();

void tearDown(void) {
  driver.delay(2000);
  driver.fillScreen(ST7789::colours::BLACK);
  driver.delay(2000);
}

TEST_CASE("Filling display test", "[ST7789Driver][rectangles]") {
  driver.fillScreen(ST7789::colours::RED);
}

TEST_CASE("Drawing pixel test", "[ST7789Driver][pixels]") {
  driver.drawPixel({150, 150}, ST7789::colours::RED);
}

TEST_CASE("Drawing pixel line", "[ST7789Driver][pixels]") {
  driver.drawPixelLine({150, 150}, 30, ST7789::colours::RED);
}

TEST_CASE("Drawing filled rectangle", "[ST7789Driver][rectangles]") {
  driver.drawFilledRectangle({50, 50}, {120, 120}, ST7789::colours::RED);
}

TEST_CASE("Drawing filled rectangles", "[ST7789Driver][rectangles]") {
  driver.drawFilledRectangle({50, 50}, {120, 120}, ST7789::colours::RED);
  driver.drawFilledRectangle({239, 239}, {80, 80}, ST7789::colours::BLUE);
}

TEST_CASE("Drawing horizontal line", "[ST7789Driver][lines]") {
  driver.drawLine({50, 50}, {50, 120}, ST7789::colours::RED);
}

TEST_CASE("Drawing vertical line", "[ST7789Driver][lines]") {
  driver.drawLine({50, 50}, {120, 50}, ST7789::colours::RED);
}

TEST_CASE("Drawing lines", "[ST7789Driver][lines]") {
  ST7789::Point center{100, 100};
  int arms = 30;
  for (int i = 0; i < arms; i++) {
    float angle = 2 * tamagotchi::ST7789::consts::PI / arms * i;
    driver.drawLine(center,
                    {center.x_ + sin(angle) * 50, center.y_ + cos(angle) * 50},
                    ST7789::colours::BLUE);
  }
}

TEST_CASE("Drawing rectangle", "[ST7789Driver][rectangles]") {
  driver.drawRectangle({50, 50}, {120, 120}, ST7789::colours::RED);
  driver.drawRectangle({239, 239}, {80, 80}, ST7789::colours::BLUE);
}

TEST_CASE("Drawing triangle", "[ST7789Driver][triangles]") {
  driver.drawTriangle({50, 50}, {120, 120}, {239, 40}, ST7789::colours::BLUE);
}

TEST_CASE("Drawing filled triangle", "[ST7789Driver][triangles]") {
  driver.drawFilledTriangle({50, 50}, {120, 120}, {239, 40},
                            ST7789::colours::BLUE);
}

TEST_CASE("Drawing circle", "[ST7789Driver][circles]") {
  driver.drawCircle({100, 100}, 50, ST7789::colours::YELLOW);
}

TEST_CASE("Drawing filled circle", "[ST7789Driver][circles]") {
  driver.drawFilledCircle({100, 100}, 50, ST7789::colours::YELLOW);
}

TEST_CASE("Drawing polygon", "[ST7789Driver][polygons]") {
  driver.drawPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW);
}

TEST_CASE("Drawing rotated polygon", "[ST7789Driver][polygons]") {
  driver.drawPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW,
                     ST7789::consts::PI / 4.0);
}

TEST_CASE("Drawing rotated polygon with many vertices",
          "[ST7789Driver][polygons]") {
  driver.drawPolygon({100, 100}, 50, 16, ST7789::colours::YELLOW,
                     ST7789::consts::PI / 4.0);
}

TEST_CASE("Drawing filled polygon", "[ST7789Driver][filled_polygons]") {
  driver.drawFilledPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW);
}

TEST_CASE("Drawing filled rotated polygon", "[ST7789Driver][filled_polygons]") {
  driver.drawFilledPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW,
                           ST7789::consts::PI / 4.0);
}

TEST_CASE("Drawing filled rotated polygon with many vertices",
          "[ST7789Driver][filled_polygons]") {
  driver.drawFilledPolygon({100, 100}, 50, 16, ST7789::colours::YELLOW,
                           ST7789::consts::PI / 4.0);
}

TEST_CASE("Drawing bitmap", "[ST7789Driver][bitmap]") {
  std::vector<bool> test{
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  false, false, true,  true,  false, false,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  false, false,
      false, true,  true,  false, false, false, true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  false, false, false, true,  true,  false, false,
      false, true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  false, false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  false, false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  false, false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  false, false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  false, false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, true,  true,  true,  true,  true,  true,  true,  false, false,
      false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, true,  true,  true,
      true,  true,  true,  false, false, false, false, false, false, false,
      false, true,  true,  false, false, false, false, false, false, false,
      false, false, true,  true,  true,  true,  true,  true,  true,  false,
      false, false, false, false, false, true,  true,  true,  true,  false,
      false, false, false, false, false, false, true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
      true,  true,  true,  true,  true,

  };
  driver.drawBitmap({100, 100}, 25, 25, ST7789::colours::RED, test);
}

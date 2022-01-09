#include <stdint.h>
#include <stdio.h>

#include <cmath>
#include <memory>

#include "EspGLScreen.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "ST7789Driver.hpp"
#include "Shapes/EspGLPolygons.hpp"
#include "Shapes/EspGLRectangles.hpp"
#include "Shapes/EspGLSimpleShapes.hpp"
#include "Shapes/EspGLSmoothEdgeShapes.hpp"
#include "Shapes/EspGLTriangles.hpp"
#include "initializeDisplay.hpp"
#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::EspGL;

constexpr int SHORT_PAUSE = 1000;

tamagotchi::ST7789::ST7789VWDriver st7789driver = initializeDisplay();
std::unique_ptr<tamagotchi::ST7789::ST7789VWDriver> ptr(&st7789driver);
tamagotchi::EspGL::Screen<uint16_t> screen(consts::WIDTH, consts::HEIGHT,
                                           std::move(ptr));
Point center(screen.width() / 2, screen.height() / 2);

TEST_GROUP(EspGLShapesTests);

TEST_SETUP(EspGLShapesTests) {}

TEST_TEAR_DOWN(EspGLShapesTests) {
  tamagotchi::EspGL::delay(SHORT_PAUSE);
  screen.fill(Colour<uint16_t>(colours::BLACK));
  tamagotchi::EspGL::delay(SHORT_PAUSE);
}

TEST(EspGLShapesTests, FillingDisplayTest) {
  screen.fill(Colour<uint16_t>(colours::RED));
}

TEST(EspGLShapesTests, DrawingPixelTest) {
  screen.screenDriver()->drawPixel(center, colours::RED);
}

TEST(EspGLShapesTests, DrawingFilledRectangle) {
  Rectangle<uint16_t> rect(center, 60, 80, Colour<uint16_t>(colours::RED));
  rect.draw(screen);
}

TEST(EspGLShapesTests, DrawingFilledRectangleWithOutline) {
  Rectangle<uint16_t> rect(center, 60, 80, Colour<uint16_t>(colours::RED),
                           Colour<uint16_t>(colours::BLUE));
  rect.draw(screen);
}

TEST(EspGLShapesTests, DrawingFilledRectangles) {
  Rectangle<uint16_t> rect(center, 60, 80, Colour<uint16_t>(colours::RED));
  Rectangle<uint16_t> rect2({150, 20}, 30, 80, Colour<uint16_t>(colours::BLUE));
  rect.draw(screen);
  rect2.draw(screen);
}

TEST(EspGLShapesTests, DrawingRectangleOutline) {
  RectangleOutline<uint16_t> rect(center, 60, 80,
                                  Colour<uint16_t>(colours::RED));
  rect.draw(screen);
}

TEST(EspGLShapesTests, DrawingRectangleOutlines) {
  RectangleOutline<uint16_t> rect(center, 60, 80,
                                  Colour<uint16_t>(colours::RED));
  RectangleOutline<uint16_t> rect2({150, 20}, 30, 80,
                                   Colour<uint16_t>(colours::BLUE));
  rect.draw(screen);
  rect2.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod1) {
  Square<uint16_t> square(center, 60, Colour<uint16_t>(colours::RED));
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod2) {
  Square<uint16_t> square(center, 60, Colour<uint16_t>(colours::RED), 0.0);
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod1WithOutline) {
  Square<uint16_t> square(center, 60, Colour<uint16_t>(colours::RED),
                          Colour<uint16_t>(colours::BLUE));
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod2WithOutline) {
  Square<uint16_t> square(center, 60, Colour<uint16_t>(colours::RED), 0.0,
                          Colour<uint16_t>(colours::BLUE));
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareOutlineMethod1) {
  SquareOutline<uint16_t> square(center, 60, Colour<uint16_t>(colours::RED));
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareOutlineMethod2) {
  SquareOutline<uint16_t> square(center, 60, Colour<uint16_t>(colours::RED),
                                 0.0);
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingLines) {
  int arms = 30;
  int lengthOfArm = 50;
  for (int i = 0; i < arms; i++) {
    float angle = 2 * M_PI / arms * i;
    Line<uint16_t> line(center,
                        {center.x_ + sin(angle) * lengthOfArm,
                         center.y_ + cos(angle) * lengthOfArm},
                        Colour<uint16_t>(colours::BLUE));
    line.draw(screen);
  }
}

TEST(EspGLShapesTests, DrawingTriangle) {
  Triangle<uint16_t> triangle{
      center, {120, 120}, {239, 40}, Colour<uint16_t>(colours::RED)};
  triangle.draw(screen);
}
TEST(EspGLShapesTests, DrawingTriangleWithOutline) {
  Triangle<uint16_t> triangle{center,
                              {120, 120},
                              {239, 40},
                              Colour<uint16_t>(colours::RED),
                              Colour<uint16_t>(colours::BLUE)};
  triangle.draw(screen);
}

TEST(EspGLShapesTests, DrawingTriangles) {
  TriangleOutline<uint16_t> triangle1{
      center, {120, 120}, {239, 40}, Colour<uint16_t>(colours::RED)};
  TriangleOutline<uint16_t> triangle2{
      {150, 150}, {120, 120}, {61, 290}, Colour<uint16_t>(colours::BLUE)};
  triangle1.draw(screen);
  triangle2.draw(screen);
}

TEST(EspGLShapesTests, DrawingTriangleOutline) {
  TriangleOutline<uint16_t> triangle{
      center, {120, 120}, {239, 40}, Colour<uint16_t>(colours::RED)};
  triangle.draw(screen);
}

TEST(EspGLShapesTests, DrawingEquilateralTriangle) {
  EquilateralTriangle<uint16_t> triangle{center, 60,
                                         Colour<uint16_t>(colours::RED)};
  triangle.draw(screen);
}

TEST(EspGLShapesTests, DrawingEquilateralTriangleWithOutline) {
  EquilateralTriangle<uint16_t> triangle{center, 60,
                                         Colour<uint16_t>(colours::RED),
                                         Colour<uint16_t>(colours::BLUE)};
  triangle.draw(screen);
}

TEST(EspGLShapesTests, DrawingEquilateralTriangleOutline) {
  EquilateralTriangleOutline<uint16_t> triangle{center, 60,
                                                Colour<uint16_t>(colours::RED)};
  triangle.draw(screen);
}

TEST(EspGLShapesTests, DrawingCircle) {
  Circle<uint16_t> circle(center, 60, Colour<uint16_t>(colours::GREEN));
  circle.draw(screen);
}
TEST(EspGLShapesTests, DrawingCircleWithOutline) {
  Circle<uint16_t> circle(center, 60, Colour<uint16_t>(colours::GREEN),
                          Colour<uint16_t>(colours::RED));
  circle.draw(screen);
}

TEST(EspGLShapesTests, DrawingCircleOutline) {
  CircleOutline<uint16_t> circle(center, 60, Colour<uint16_t>(colours::GREEN));
  circle.draw(screen);
}

TEST(EspGLShapesTests, DrawingEllipse) {
  Ellipse<uint16_t> ellipse(center, 60, 80, Colour<uint16_t>(colours::GREEN));
  ellipse.draw(screen);
}
TEST(EspGLShapesTests, DrawingEllipseWithOutline) {
  Ellipse<uint16_t> ellipse(center, 60, 80, Colour<uint16_t>(colours::GREEN),
                            Colour<uint16_t>(colours::RED));
  ellipse.draw(screen);
}

TEST(EspGLShapesTests, DrawingEllipseOutline) {
  EllipseOutline<uint16_t> ellipse(center, 60, 80,
                                   Colour<uint16_t>(colours::GREEN));
  ellipse.draw(screen);
}

TEST(EspGLShapesTests, DrawingRegularPolygon) {
  RegularPolygon<uint16_t, 4> polygon(center, 50,
                                      Colour<uint16_t>(colours::GREEN));
  polygon.draw(screen);
}

TEST(EspGLShapesTests, DrawingRegularPolygonWithOutline) {
  RegularPolygon<uint16_t, 4> polygon(center, 50,
                                      Colour<uint16_t>(colours::GREEN),
                                      Colour<uint16_t>(colours::YELLOW));
  polygon.draw(screen);
}

TEST(EspGLShapesTests, DrawingRotatedRegularPolygonWithOutline) {
  RegularPolygon<uint16_t, 4> polygon(center, 50,
                                      Colour<uint16_t>(colours::GREEN),
                                      Colour<uint16_t>(colours::YELLOW), 20);
  polygon.draw(screen);
}

TEST(EspGLShapesTests, DrawingRegularPolygonOutline) {
  RegularPolygonOutline<uint16_t, 4> polygon(center, 50,
                                             Colour<uint16_t>(colours::GREEN));
  polygon.draw(screen);
}

TEST(EspGLShapesTests, DrawingRotatedRegularPolygonOutline) {
  RegularPolygonOutline<uint16_t, 4> polygon(
      center, 50, Colour<uint16_t>(colours::GREEN), 20);
  polygon.draw(screen);
}

TEST(EspGLShapesTests, DrawingBitmap) {
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
  Bitmap bitmap(50, 50, test);
  bitmap.draw(screen, center, Colour<uint16_t>(colours::GREEN));
}

TEST_GROUP_RUNNER(EspGLShapesTests) {
  RUN_TEST_CASE(EspGLShapesTests, FillingDisplayTest)
  RUN_TEST_CASE(EspGLShapesTests, DrawingPixelTest)
  RUN_TEST_CASE(EspGLShapesTests, DrawingFilledRectangle)
  RUN_TEST_CASE(EspGLShapesTests, DrawingFilledRectangleWithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingFilledRectangles)
  RUN_TEST_CASE(EspGLShapesTests, DrawingRectangleOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingRectangleOutlines)
  RUN_TEST_CASE(EspGLShapesTests, DrawingSquareMethod1)
  RUN_TEST_CASE(EspGLShapesTests, DrawingSquareMethod2)
  RUN_TEST_CASE(EspGLShapesTests, DrawingSquareMethod1WithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingSquareMethod2WithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingSquareOutlineMethod1)
  RUN_TEST_CASE(EspGLShapesTests, DrawingSquareOutlineMethod2)
  RUN_TEST_CASE(EspGLShapesTests, DrawingLines)
  RUN_TEST_CASE(EspGLShapesTests, DrawingTriangle)
  RUN_TEST_CASE(EspGLShapesTests, DrawingTriangleWithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingTriangles)
  RUN_TEST_CASE(EspGLShapesTests, DrawingTriangleOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingEquilateralTriangle)
  RUN_TEST_CASE(EspGLShapesTests, DrawingEquilateralTriangleWithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingEquilateralTriangleOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingCircle)
  RUN_TEST_CASE(EspGLShapesTests, DrawingCircleWithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingCircleOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingEllipse)
  RUN_TEST_CASE(EspGLShapesTests, DrawingEllipseWithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingEllipseOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingRegularPolygon)
  RUN_TEST_CASE(EspGLShapesTests, DrawingRegularPolygonWithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingRotatedRegularPolygonWithOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingRegularPolygonOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingRotatedRegularPolygonOutline)
  RUN_TEST_CASE(EspGLShapesTests, DrawingBitmap)
}
#include <stdint.h>
#include <stdio.h>

#include <memory>

#include "EspGLScreen.hpp"
#include "ST7789Driver.hpp"
#include "Shapes/EspGLPolygons.hpp"
#include "Shapes/EspGLRectangles.hpp"
#include "Shapes/EspGLSimpleShapes.hpp"
#include "Shapes/EspGLSmoothEdgeShapes.hpp"
#include "Shapes/EspGLTriangles.hpp"
#include "testEspGLUtils.hpp"
#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::ST7789;
using namespace tamagotchi::EspGL;

constexpr int SHORT_PAUSE = 1000;

ST7789VWDriver st7789driver = initializeDisplay();
std::unique_ptr<ST7789VWDriver> ptr(&st7789driver);
tamagotchi::EspGL::Screen<uint16_t> screen(consts::WIDTH, consts::HEIGHT,
                                           std::move(ptr));

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
  screen.screenDriver()->drawPixel({150, 150}, colours::RED);
}

TEST(EspGLShapesTests, DrawingFilledRectangle) {
  Rectangle<uint16_t> rect({50, 50}, 60, 80, Colour<uint16_t>(colours::RED));
  rect.draw(screen);
}

TEST(EspGLShapesTests, DrawingFilledRectangleWithOutline) {
  Rectangle<uint16_t> rect({50, 50}, 60, 80, Colour<uint16_t>(colours::RED),
                           Colour<uint16_t>(colours::BLUE));
  rect.draw(screen);
}

TEST(EspGLShapesTests, DrawingFilledRectangles) {
  Rectangle<uint16_t> rect({50, 50}, 60, 80, Colour<uint16_t>(colours::RED));
  Rectangle<uint16_t> rect2({150, 20}, 30, 80, Colour<uint16_t>(colours::BLUE));
  rect.draw(screen);
  rect2.draw(screen);
}

TEST(EspGLShapesTests, DrawingRectangleOutline) {
  RectangleOutline<uint16_t> rect({50, 50}, 60, 80,
                                  Colour<uint16_t>(colours::RED));
  rect.draw(screen);
}

TEST(EspGLShapesTests, DrawingRectangleOutlines) {
  RectangleOutline<uint16_t> rect({50, 50}, 60, 80,
                                  Colour<uint16_t>(colours::RED));
  RectangleOutline<uint16_t> rect2({150, 20}, 30, 80,
                                   Colour<uint16_t>(colours::BLUE));
  rect.draw(screen);
  rect2.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod1) {
  Square<uint16_t> square({50, 50}, 60, Colour<uint16_t>(colours::RED));
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod2) {
  Square<uint16_t> square({50, 50}, 60, Colour<uint16_t>(colours::RED), 0.0);
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod1WithOutline) {
  Square<uint16_t> square({50, 50}, 60, Colour<uint16_t>(colours::RED),
                          Colour<uint16_t>(colours::BLUE));
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareMethod2WithOutline) {
  Square<uint16_t> square({50, 50}, 60, Colour<uint16_t>(colours::RED),
                          Colour<uint16_t>(colours::BLUE), 0.0);
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareOutlineMethod1) {
  SquareOutline<uint16_t> square({50, 50}, 60, Colour<uint16_t>(colours::RED));
  square.draw(screen);
}

TEST(EspGLShapesTests, DrawingSquareOutlineMethod2) {
  SquareOutline<uint16_t> square({50, 50}, 60, Colour<uint16_t>(colours::RED),
                                 0.0);
  square.draw(screen);
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
}
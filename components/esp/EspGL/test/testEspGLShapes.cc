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

constexpr int SHORT_PAUSE = 5000;

ST7789VWDriver st7789driver = initializeDisplay();
std::unique_ptr<ST7789VWDriver> ptr(&st7789driver);
tamagotchi::EspGL::Screen<uint16_t> screen(consts::WIDTH, consts::HEIGHT,
                                           std::move(ptr));

TEST_GROUP(EspGLShapesTests);

TEST_SETUP(EspGLShapesTests) {}

TEST_TEAR_DOWN(EspGLShapesTests) {
  tamagotchi::EspGL::delay(SHORT_PAUSE);
  // screen.fill(Colour<uint16_t>(colours::BLACK));
  tamagotchi::EspGL::delay(SHORT_PAUSE);
}

TEST(EspGLShapesTests, FillingDisplayTest) {
  screen.fill(Colour<uint16_t>(colours::RED));
}

TEST_GROUP_RUNNER(EspGLShapesTests) {
  RUN_TEST_CASE(EspGLShapesTests, FillingDisplayTest)
}
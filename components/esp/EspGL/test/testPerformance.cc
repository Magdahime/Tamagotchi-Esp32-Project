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
#include "driver/timer.h"
#include "initializeDisplay.hpp"
#include "unity.h"
#include "unity_fixture.h"

#define TIMER_DIVIDER (16)  //  Hardware timer clock divider
#define TIMER_SCALE \
  (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds

using namespace tamagotchi::EspGL;

extern tamagotchi::ST7789::ST7789VWDriver st7789driver;
extern tamagotchi::EspGL::Screen<uint16_t> screen;

void timerInit(timer_group_t group, timer_idx_t timer,
               timer_autoreload_t autoReload) {
  timer_config_t config = {.alarm_en = TIMER_ALARM_DIS,
                           .counter_en = TIMER_PAUSE,
                           .intr_type = TIMER_INTR_LEVEL,
                           .counter_dir = TIMER_COUNT_UP,
                           .auto_reload = autoReload,
                           .divider = TIMER_DIVIDER};

  timer_init(group, timer, &config);
  ESP_LOGI("EspGLPerformanceTests", "Timer init successfull");
  timer_set_counter_value(group, timer, 0);
}

/// SETUP
constexpr int numberOfLoops = 1000;
///

TEST_SETUP(EspGLPerformanceTests) {
  timerInit(TIMER_GROUP_0, TIMER_0, TIMER_AUTORELOAD_EN);
}

TEST_GROUP(EspGLPerformanceTests);

TEST_TEAR_DOWN(EspGLPerformanceTests) {}

TEST(EspGLPerformanceTests, FillingDisplayPerformanceTest) {
  double timerValue;
  for (auto i = 0; i < numberOfLoops; i++) {
    timer_start(TIMER_GROUP_0, TIMER_0);
    screen.fill(Colour<uint16_t>(colours::RED));
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_get_counter_time_sec(TIMER_GROUP_0, TIMER_0, &timerValue);
    // printf("%f\n", timerValue);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
  }
}

TEST(EspGLPerformanceTests, DrawingSquares) {
  double timerValue;
  for (auto i = 0; i < numberOfLoops; i++) {
    timer_start(TIMER_GROUP_0, TIMER_0);
    Rectangle<uint16_t> rect({0, 0}, 120, 100, Colour<uint16_t>(colours::RED));
    rect.draw(screen);
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_get_counter_time_sec(TIMER_GROUP_0, TIMER_0, &timerValue);
    printf("%f\n", timerValue);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
  }
}

TEST(EspGLPerformanceTests, DrawingCircles) {
  double timerValue;
  for (auto i = 0; i < numberOfLoops; i++) {
    timer_start(TIMER_GROUP_0, TIMER_0);
    Circle<uint16_t> circle({100, 100}, 62, Colour<uint16_t>(colours::GREEN));
    circle.draw(screen);
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_get_counter_time_sec(TIMER_GROUP_0, TIMER_0, &timerValue);
    printf("%f\n", timerValue);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
  }
}

TEST(EspGLPerformanceTests, DrawingTriangles) {
  double timerValue;
  for (auto i = 0; i < numberOfLoops; i++) {
    timer_start(TIMER_GROUP_0, TIMER_0);
    Triangle<uint16_t> triangle{
        {0, 0}, {0, 120}, {200, 120}, Colour<uint16_t>(colours::RED)};
    triangle.draw(screen);
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_get_counter_time_sec(TIMER_GROUP_0, TIMER_0, &timerValue);
    printf("%f\n", timerValue);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
  }
}

TEST(EspGLPerformanceTests, DrawingEllipses) {
  double timerValue;
  for (auto i = 0; i < numberOfLoops; i++) {
    timer_start(TIMER_GROUP_0, TIMER_0);
    Ellipse<uint16_t> ellipse({100, 150}, 30, 128,
                              Colour<uint16_t>(colours::GREEN));
    ellipse.draw(screen);
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_get_counter_time_sec(TIMER_GROUP_0, TIMER_0, &timerValue);
    printf("%f\n", timerValue);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
  }
}

TEST_GROUP_RUNNER(EspGLPerformanceTests) {
  // RUN_TEST_CASE(EspGLPerformanceTests, FillingDisplayPerformanceTest)
  // RUN_TEST_CASE(EspGLPerformanceTests, DrawingSquares)
  // RUN_TEST_CASE(EspGLPerformanceTests, DrawingCircles)
  // RUN_TEST_CASE(EspGLPerformanceTests, DrawingTriangles)
  RUN_TEST_CASE(EspGLPerformanceTests, DrawingEllipses)
}
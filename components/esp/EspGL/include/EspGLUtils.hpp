#pragma once

#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <tuple>
#include <utility>

#include "EspGLUtils.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


namespace tamagotchi {
namespace EspGL {


struct Point {
  int32_t x_;
  int32_t y_;
  template <typename T, typename Y>
  Point(T x, Y y) : x_(std::round(x)), y_(std::round(y)) {}
};

enum class Coordinate { X, Y };

std::tuple<Point, Point, Point> sort3Points(const Point &a, const Point &b,
                                            const Point &c, Coordinate orderBy);
Point straightLineEquation(const Point &start, const Point &end,
                           const Point &search, const Coordinate coordinate);
void delay(const uint32_t ms);

struct RGB888 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

template <typename ColourRepresentation>
class Colour {
 public:
  Colour(ColourRepresentation colour) : value_(colour) {}
  Colour(uint8_t r, uint8_t g, uint8_t b) : value_(convertRGB(r, g, b)) {}
  RGB888 getRGB();
  inline ColourRepresentation value() { return value_; }

 private:
  ColourRepresentation value_;
  ColourRepresentation convertRGB(uint8_t r, uint8_t g, uint8_t b);
};

using Colour16 = Colour<uint16_t>;

}  // namespace EspGL
}  // namespace tamagotchi
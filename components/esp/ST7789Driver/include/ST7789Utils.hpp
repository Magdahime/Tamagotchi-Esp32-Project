#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>

namespace tamagotchi {

namespace ST7789 {

struct Point {
  int16_t x_;
  int16_t y_;
  template <typename T>
  Point(T x, T y) : x_(std::round(x)), y_(std::round(y)) {}
};

enum class Coordinate { X, Y };

std::tuple<Point, Point, Point> sort3Points(const Point &a, const Point &b,
                                            const Point &c, Coordinate orderBy);
Point straightLineEquation(const Point &start, const Point &end,
                           const Point &search, Coordinate coordinate);
}  // namespace ST7789
}  // namespace tamagotchi

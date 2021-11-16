#include "ST7789Utils.hpp"

#include <stdint.h>

#include <vector>

namespace tamagotchi {
namespace ST7789 {
std::tuple<Point, Point, Point> sort3Points(const Point &a, const Point &b,
                                            const Point &c,
                                            Coordinate orderBy) {
  std::vector<Point> points{a, b, c};
  std::sort(points.begin(), points.end(), [&](const Point &a, const Point &b) {
    switch (orderBy) {
      case Coordinate::X:
        return a.x_ > b.x_;
      case Coordinate::Y:
        return a.y_ > b.y_;
      default:
        return false;
    }
  });
  return {points[0], points[1], points[2]};
}

Point straightLineEquation(const Point &start, const Point &end,
                           const Point &search, Coordinate coordinate) {
  if (end.x_ == start.x_) {
    switch (coordinate) {
      case Coordinate::X:
        return Point{start.x_, search.y_};
      case Coordinate::Y:
        return Point{search.x_, static_cast<int16_t>((start.y_ + end.y_) / 2)};
      default:
        return Point{0, 0};
    }
  }

  if (end.y_ == start.y_) {
    switch (coordinate) {
      case Coordinate::X:
        return Point{static_cast<int16_t>((start.x_ + end.x_) / 2), start.y_};
      case Coordinate::Y:
        return Point{search.x_, start.y_};
      default:
        return Point{0, 0};
    }
  }

  double m = static_cast<double>(end.y_ - start.y_) /
             static_cast<double>(end.x_ - start.x_);
  double c = start.y_ - m * start.x_;
  switch (coordinate) {
    case Coordinate::X:
      return Point{static_cast<int16_t>(std::lround((search.y_ - c) / m)),
                   search.y_};
    case Coordinate::Y:
      return Point{search.x_,
                   static_cast<int16_t>(std::lround(search.x_ * m + c))};
    default:
      return Point{0, 0};
  }
  return Point{0, 0};
}

}  // namespace ST7789
}  // namespace tamagotchi
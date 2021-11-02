#include "ST7789Utils.hpp"

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
        return a.x > b.x;
      case Coordinate::Y:
        return a.y > b.y;
      default:
        return false;
    }
  });
  return {points[0], points[1], points[2]};
}

Point straightLineEquation(const Point &start, const Point &end,
                           const Point &search, Coordinate coordinate) {
  double m = static_cast<double>(end.y - start.y) /
             static_cast<double>(end.x - start.x);
  double c = start.y - m * start.x;
  switch (coordinate) {
    case Coordinate::X:
      return Point{static_cast<int16_t>(std::lround((search.y - c) / m)),
                   search.y};
    case Coordinate::Y:
      return Point{search.x,
                   static_cast<int16_t>(std::lround(search.x * m + c))};
  }
}

}  // namespace ST7789
}  // namespace tamagotchi
#include "EspGLUtils.hpp"

#include <stdint.h>

#include <vector>

namespace tamagotchi {
namespace EspGL {

void delay(const uint32_t ms) {
  TickType_t delay = ms / portTICK_PERIOD_MS;
  vTaskDelay(delay);
}

std::tuple<Vect2, Vect2, Vect2> sort3Points(const Vect2 &a, const Vect2 &b,
                                            const Vect2 &c,
                                            const Coordinate orderBy) {
  std::vector<Vect2> points{a, b, c};
  std::sort(points.begin(), points.end(), [&](const Vect2 &a, const Vect2 &b) {
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

Vect2 straightLineEquation(const Vect2 &start, const Vect2 &end,
                           const Vect2 &search, Coordinate coordinate) {
  if (end.x_ == start.x_) {
    switch (coordinate) {
      case Coordinate::X:
        return Vect2{start.x_, search.y_};
      case Coordinate::Y:
        return Vect2{search.x_, static_cast<int16_t>((start.y_ + end.y_) / 2)};
      default:
        return Vect2{0, 0};
    }
  }

  if (end.y_ == start.y_) {
    switch (coordinate) {
      case Coordinate::X:
        return Vect2{static_cast<int16_t>((start.x_ + end.x_) / 2), start.y_};
      case Coordinate::Y:
        return Vect2{search.x_, start.y_};
      default:
        return Vect2{0, 0};
    }
  }

  double m = static_cast<double>(end.y_ - start.y_) /
             static_cast<double>(end.x_ - start.x_);
  double c = start.y_ - m * start.x_;
  switch (coordinate) {
    case Coordinate::X:
      return Vect2{static_cast<int16_t>(std::lround((search.y_ - c) / m)),
                   search.y_};
    case Coordinate::Y:
      return Vect2{search.x_,
                   static_cast<int16_t>(std::lround(search.x_ * m + c))};
    default:
      return Vect2{0, 0};
  }
  return Vect2{0, 0};
}

}  // namespace EspGL
}  // namespace tamagotchi
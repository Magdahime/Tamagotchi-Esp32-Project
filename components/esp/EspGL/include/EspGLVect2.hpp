#pragma once

#include <stdint.h>

#include <cmath>
#include <utility>

namespace tamagotchi {
namespace EspGL {
struct Vect2 {
  int32_t x_;
  int32_t y_;
  template <typename T, typename Y>
  Vect2(T x, Y y) : x_(std::round(x)), y_(std::round(y)) {}
  Vect2() : x_(0), y_(0) {}
  Vect2 operator+(Vect2 const &vect2) {
    return Vect2(x_ + vect2.x_, y_ + vect2.y_);
  }
  Vect2 operator-(Vect2 const &vect2) {
    return Vect2(x_ - vect2.x_, y_ - vect2.y_);
  }
  template <
      typename ArithmeticType,
      typename = typename std::enable_if<
          std::is_arithmetic<ArithmeticType>::value, ArithmeticType>::type>
  Vect2 operator+(ArithmeticType constant) {
    return Vect2(x_ + constant, y_ + constant);
  }
  template <
      typename ArithmeticType,
      typename = typename std::enable_if<
          std::is_arithmetic<ArithmeticType>::value, ArithmeticType>::type>
  Vect2 operator-(ArithmeticType constant) {
    return Vect2(x_ - constant, y_ - constant);
  }
};

}  // namespace EspGL
}  // namespace tamagotchi
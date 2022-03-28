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

struct Vect2 {
  int32_t x_;
  int32_t y_;
  template <typename T, typename Y>
  Vect2(T x, Y y) : x_(std::round(x)), y_(std::round(y)) {}
  Vect2() : x_(0), y_(0) {}
  Vect2 operator+(Vect2 const &point2);
  Vect2 operator-(Vect2 const &point2);
  template <
      typename ArithmeticType,
      typename = typename std::enable_if<
          std::is_arithmetic<ArithmeticType>::value, ArithmeticType>::type>
  Vect2 operator+(ArithmeticType constant);
  template <
      typename ArithmeticType,
      typename = typename std::enable_if<
          std::is_arithmetic<ArithmeticType>::value, ArithmeticType>::type>
  Vect2 operator-(ArithmeticType constant);
};

enum class Coordinate { X, Y };

std::tuple<Vect2, Vect2, Vect2> sort3Points(const Vect2 &a, const Vect2 &b,
                                            const Vect2 &c, Coordinate orderBy);
Vect2 straightLineEquation(const Vect2 &start, const Vect2 &end,
                           const Vect2 &search, const Coordinate coordinate);
void delay(const uint32_t ms);

struct RGB888 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

template <typename ColourRepresentation>
class Colour {
 public:
  Colour() = default;
  Colour(ColourRepresentation colour) : value_(colour) {}
  inline ColourRepresentation value() const { return value_; }
  inline void setValue(ColourRepresentation newValue) { value_ = newValue; }
  inline ColourRepresentation getNegativeColourValue() { return -value_; }

 private:
  ColourRepresentation value_;
};

namespace colours {
constexpr uint16_t RED = 0xf800;
constexpr uint16_t GREEN = 0x07e0;
constexpr uint16_t BLUE = 0x001f;
constexpr uint16_t BLACK = 0x0000;
constexpr uint16_t WHITE = 0xffff;
constexpr uint16_t GRAY = 0x8c51;
constexpr uint16_t YELLOW = 0xFFE0;
constexpr uint16_t CYAN = 0x07FF;
constexpr uint16_t PURPLE = 0xF81F;
}  // namespace colours

using Colour16 = Colour<uint16_t>;

}  // namespace EspGL
}  // namespace tamagotchi
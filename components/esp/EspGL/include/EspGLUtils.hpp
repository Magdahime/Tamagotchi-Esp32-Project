#pragma once

#include <stdint.h>
namespace tamagotchi {
namespace EspGL {

struct RGB888 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class Colour {
 public:
  Colour(uint16_t colour) : value_(colour) {}
  Colour(uint8_t r, uint8_t g, uint8_t b) : value_(convertRGB(r, g, b)) {}
  RGB888 getRGB();
  inline uint16_t value() { return value_; }

 private:
  uint16_t value_;
  uint16_t convertRGB(uint8_t r, uint8_t g, uint8_t b);
};

}  // namespace EspGL
}  // namespace tamagotchi
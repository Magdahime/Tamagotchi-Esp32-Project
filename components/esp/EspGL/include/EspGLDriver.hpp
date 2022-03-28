#pragma once
#include <stdint.h>
#include <stdio.h>

#include <vector>

#include "EspGLUtils.hpp"


namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Driver {
 public:
  virtual void drawPixel(const Vect2& point, ColourRepresentation colour) = 0;
  virtual void writePixelArea(int16_t startX, int16_t endX, int16_t startY,
                              int16_t endY, ColourRepresentation colour) = 0;
  virtual void writePixelArea(int16_t startX, int16_t endX, int16_t startY,
                              int16_t endY,
                              std::vector<ColourRepresentation>& data) = 0;
  virtual ~Driver() = default;
 private:
  virtual void writeCommand(uint8_t command) = 0;
  virtual void writeBytes(const uint8_t* bytes, size_t size) = 0;
  virtual void writeByte(const uint8_t byte) = 0;
  virtual void writeColour(ColourRepresentation colour) = 0;
  virtual void writeColour(ColourRepresentation colour, size_t size) = 0;
};

}  // namespace EspGL
}  // namespace tamagotchi

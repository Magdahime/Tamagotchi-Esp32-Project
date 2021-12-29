#pragma once
#include <stdint.h>

#include <vector>

#include "EspGL.hpp"
#include "EspGLUtils.hpp"
#include "EspGlScreen.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Bitmap {
 public:
  Bitmap(size_t sizeX, size_t sizeY, Colour<ColourRepresentation> colour,
         std::vector<bool> bitmap)
      : sizeX_(std::move(sizeX)),
        sizeY_(std::move(sizeY)),
        colour_(std::move(colour)),
        bitmap_(std::move(bitmap)) {}
  void draw(Screen<ColourRepresentation>& target, const Point& start);

 private:
  size_t sizeX_;
  size_t sizeY_;
  Colour<ColourRepresentation> colour_;
  std::vector<bool> bitmap_;
};

template <typename ColourRepresentation>
void Bitmap<ColourRepresentation>::draw(Screen<ColourRepresentation>& target,
                                        const Point& start) {
  ESP_LOGD(TAG_, "Drawing bitmap of size (%zu, %zu)in colour: 0x%X", sizeX_,
           sizeY_, colour_.value());
  uint16_t countX = 0, countY = 0;
  for (const auto& pixel : bitmap_) {
    if (pixel) {
      target.screenDriver()->drawPixel({start.x_ + countX, start.y_ + countY},
                                       colour_.value());
    }
    if (++countX == sizeX_) {
      countY++;
      countX = 0;
    }
  }
}

}  // namespace EspGL
}  // namespace tamagotchi
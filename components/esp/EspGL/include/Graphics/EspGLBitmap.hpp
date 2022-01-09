#pragma once
#include <stdint.h>

#include <stdexcept>
#include <vector>

#include "EspGL.hpp"
#include "EspGLUtils.hpp"
#include "EspGlScreen.hpp"

namespace tamagotchi {
namespace EspGL {

class Bitmap {
 public:
  Bitmap(size_t sizeX, size_t sizeY, std::vector<bool> bitmap)
      : sizeX_(std::move(sizeX)),
        sizeY_(std::move(sizeY)),
        bitmap_(std::move(bitmap)) {
    if (bitmap_.size() != sizeX_ * sizeY_) {
      throw std::runtime_error(
          "Invalid std::vector<bool> passed to Bitmap constructor. Size is not "
          "equal to provided values.");
    }
  }
  Bitmap() = default;
  template <typename ColourRepresentation>
  void draw(Screen<ColourRepresentation>& target, const Point& start,
            Colour<ColourRepresentation> colour);
  void overlay(Bitmap& bitmap, Point& start);

  inline size_t sizeX() { return sizeX_; }
  inline size_t sizeY() { return sizeY_; }
  inline std::vector<bool>& bitmap() { return bitmap_; };
  inline bool empty() const { return bitmap_.empty(); }
  inline bool at(int64_t index) const { return bitmap_[index]; }

 private:
  size_t sizeX_;
  size_t sizeY_;
  std::vector<bool> bitmap_;
};

template <typename ColourRepresentation>
void Bitmap::draw(Screen<ColourRepresentation>& target, const Point& start,
                  Colour<ColourRepresentation> colour) {
  ESP_LOGD(TAG_, "Drawing bitmap of size (%zu, %zu)in colour: 0x%X", sizeX_,
           sizeY_, colour.value());
  uint16_t countX = 0, countY = 0;
  for (const auto& pixel : bitmap_) {
    if (pixel) {
      target.screenDriver()->drawPixel({start.x_ + countX, start.y_ + countY},
                                       colour.value());
    }
    if (++countX == sizeX_) {
      countY++;
      countX = 0;
    }
  }
}

}  // namespace EspGL
}  // namespace tamagotchi
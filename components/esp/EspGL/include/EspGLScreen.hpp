#pragma once
#include <stdint.h>
#include <stdio.h>

#include <memory>

#include "EspGLDriver.hpp"

namespace tamagotchi {
namespace EspGL {
template <typename ColourRepresentation>
class Screen {
 public:
  Screen(int64_t width, int64_t height,
         std::unique_ptr<Driver<ColourRepresentation>> driver)
      : width_(width), height_(height), screenDriver_(std::move(driver)) {}
  Screen() = default;
  inline int64_t width() { return width_; }
  inline int64_t height() { return height_; }
  inline Driver<ColourRepresentation>* screenDriver() {
    return screenDriver_.get();
  }
  inline void fill(Colour<ColourRepresentation> colour) {
    screenDriver_->writePixelArea(0, width_ - 1, 0, height_ - 1,
                                  colour.value());
  }

 private:
  int64_t width_;
  int64_t height_;
  std::unique_ptr<Driver<ColourRepresentation>> screenDriver_;
};

}  // namespace EspGL
}  // namespace tamagotchi
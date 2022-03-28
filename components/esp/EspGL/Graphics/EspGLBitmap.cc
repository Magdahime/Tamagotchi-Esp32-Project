#include "Graphics/EspGLBitmap.hpp"

namespace tamagotchi {
namespace EspGL {

void Bitmap::overlay(Bitmap& bitmap, Vect2& start) {
  if (!bitmap.empty()) {
    if (bitmap_.empty()) {
      throw std::runtime_error("Primary bitmap in overlay() method is empty!");
    }

    if (start.x_ + bitmap.sizeX() > sizeX_ ||
        start.y_ + bitmap.sizeY() > sizeY_) {
      throw std::runtime_error(
          "Bitmap given to overlay() method has invalid dimensions or point is "
          "invalid!");
    }

    for (auto i = 0; i < bitmap.sizeY(); ++i) {
      for (auto j = 0; j < bitmap.sizeX(); ++j) {
        bitmap_[(i + start.y_) * sizeY_ + (j + start.x_)] =
            bitmap.at(j * bitmap.sizeY() + i);
      }
    }
  }
}

}  // namespace EspGL
}  // namespace tamagotchi
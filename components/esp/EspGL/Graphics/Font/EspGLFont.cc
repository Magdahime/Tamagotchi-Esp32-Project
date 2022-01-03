#include "Graphics/Font/EspGLFont.hpp"

#include <stdexcept>

#include "Graphics/EspGLBitmap.hpp"

namespace tamagotchi {
namespace EspGL {
Bitmap Font::at(char letter) const {
  try {
    return letters_.at(letter);
  } catch (const std::out_of_range& e) {
    return null();
  }
  return null();
}
}  // namespace EspGL
}  // namespace tamagotchi

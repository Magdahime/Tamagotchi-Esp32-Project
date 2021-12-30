#pragma once
#include <map>

#include "../EspGLBitmap.hpp"

namespace tamagotchi {
namespace EspGL {

class Font {
 public:
 private:
  std::map<char, Bitmap> letters;
};

}  // namespace EspGL
}  // namespace tamagotchi
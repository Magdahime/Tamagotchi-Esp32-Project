#pragma once
#include <map>

#include "../EspGLBitmap.hpp"

namespace tamagotchi {
namespace EspGL {

class Font {
 public:
  Font(std::map<char, Bitmap>& letters) : letters_(std::move(letters)) {}

 private:
  std::map<char, Bitmap> letters_;
};

}  // namespace EspGL
}  // namespace tamagotchi
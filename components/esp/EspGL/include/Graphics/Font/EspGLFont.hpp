#pragma once
#include <map>
#include <string>

#include "../EspGLBitmap.hpp"

namespace tamagotchi {
namespace EspGL {

class Font {
 public:
  Font(std::map<std::string, Bitmap>& letters) : letters_(std::move(letters)) {}

 private:
  std::map<std::string, Bitmap> letters_;
};

}  // namespace EspGL
}  // namespace tamagotchi
#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../EspGLBitmap.hpp"
#include "EspGLFont.hpp"

namespace tamagotchi {
namespace EspGL {

class FontLoader {
 public:
  Font load(std::string filename);

 private:
 
  void parseMagicNumber();
  Bitmap parseBitmap();
  std::pair<uint16_t, uint16_t> parseDimensions();
  std::vector<char> parseLetters();
  void parseComment();
};

}  // namespace EspGL
}  // namespace tamagotchi
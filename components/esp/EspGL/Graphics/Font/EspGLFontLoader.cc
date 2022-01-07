#include "Graphics/Font/EspGLFontLoader.hpp"

#include <ctype.h>
#include <stdint.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

#include "Graphics/EspGLBitmap.hpp"
#include "Graphics/Font/EspGLFont.hpp"

namespace tamagotchi {
namespace EspGL {

Font FontLoader::load() {
  std::map<char, Bitmap> fontMap;
  Bitmap null;
  bitmapLoader_.parseMagicNumber();
  auto dimensions = bitmapLoader_.parseDimensions();
  uint32_t fontHeight = dimensions.second;
  int64_t symbolsNumber = bitmapLoader_.parseBitmapNumber();
  std::vector<std::string> letters = bitmapLoader_.parseSymbols(symbolsNumber);
  for (auto& letter : letters) {
    if (letter == "null") {
      null = Bitmap(
          dimensions.first, dimensions.second,
          bitmapLoader_.parseBitmap(dimensions.first, dimensions.second));
    } else {
      fontMap.insert(std::make_pair(
          letter.front(), Bitmap(dimensions.first, dimensions.second,
                                 bitmapLoader_.parseBitmap(
                                     dimensions.first, dimensions.second))));
    }
  }
  if (!null.empty()) {
    return Font(fontHeight, fontMap, null);
  }
  return Font(fontHeight, fontMap);
}

}  // namespace EspGL
}  // namespace tamagotchi
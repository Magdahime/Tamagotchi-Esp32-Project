#pragma once
#include <stdint.h>

#include <cstdint>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "../EspGLBitmap.hpp"
#include "EspGLFont.hpp"

namespace tamagotchi {
namespace EspGL {

class FontLoader {
 public:
  FontLoader(std::fstream fileHandle)
      : fileHandle_(std::move(fileHandle)),
        currentCharacter_(),
        characterCounter_(0) {
    nextCharacter();
  }
  Font load(std::string filename);
  std::string parseMagicNumber();
  Bitmap parseBitmap(size_t dim1, size_t dim2);
  std::pair<size_t, size_t> parseDimensions();
  std::vector<std::string> parseLetters();
  void parseComment();

 private:
  std::fstream fileHandle_;
  std::string currentCharacter_;
  std::uint64_t characterCounter_;
  inline void nextCharacter() { fileHandle_ >> currentCharacter_; }
  void ignoreWhitespaces();
};

}  // namespace EspGL
}  // namespace tamagotchi
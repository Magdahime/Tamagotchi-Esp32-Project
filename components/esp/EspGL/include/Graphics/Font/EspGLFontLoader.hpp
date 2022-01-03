#pragma once
#include <stdint.h>

#include <cstdint>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "../EspGLBitmap.hpp"
#include "EspGLFont.hpp"
#include "EspGLUtils.hpp"

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
  FontLoader() = default;
  Font load();
  std::string parseMagicNumber();
  Bitmap parseBitmap(size_t dim1, size_t dim2);
  std::pair<size_t, size_t> parseDimensions();
  std::vector<std::string> parseLetters();
  void parseComment();

  inline const std::fstream& fileHandle() const { return fileHandle_; }
  inline const std::string& currentCharacter() const {
    return currentCharacter_;
  }
  inline std::uint64_t characterCounter() const { return characterCounter_; }

 private:
  std::fstream fileHandle_;
  std::string currentCharacter_;
  std::uint64_t characterCounter_;
  inline void nextCharacter() {
    characterCounter_ += currentCharacter_.size();
    fileHandle_ >> currentCharacter_;
  }
  void ignoreWhitespaces();
};

}  // namespace EspGL
}  // namespace tamagotchi
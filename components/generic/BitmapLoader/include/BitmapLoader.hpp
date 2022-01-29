#pragma once

#include <stdint.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

namespace tamagotchi {

namespace BitmapLoader {

class BitmapLoader {
 public:
  BitmapLoader(std::fstream fileHandle)
      : fileHandle_(std::move(fileHandle)),
        currentCharacter_(),
        characterCounter_(0) {
    nextCharacter();
  }
  BitmapLoader() = default;
  std::string parseMagicNumber();
  std::vector<bool> parseBitmap(size_t dim1, size_t dim2);
  std::pair<size_t, size_t> parseDimensions();
  std::vector<std::string> parseSymbols(int64_t bitmapNum);
  int64_t parseBitmapNumber();
  void parseComment();

  inline std::fstream& fileHandle() { return fileHandle_; }
  inline std::string& currentCharacter() { return currentCharacter_; }
  inline std::uint64_t characterCounter() const { return characterCounter_; }

 private:
  std::fstream fileHandle_;
  std::string currentCharacter_;
  uint64_t characterCounter_;
  void ignoreWhitespaces();
  inline void nextCharacter() {
    characterCounter_ += currentCharacter_.size();
    fileHandle_ >> currentCharacter_;
  }
};

}  // namespace BitmapLoader

}  // namespace tamagotchi
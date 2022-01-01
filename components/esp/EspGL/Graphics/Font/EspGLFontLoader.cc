#include "Graphics/Font/EspGLFontLoader.hpp"

#include <ctype.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

#include "Graphics/EspGLBitmap.hpp"
#include "Graphics/Font/EspGLFont.hpp"

namespace tamagotchi {
namespace EspGL {

Font FontLoader::load(std::string filename) {
  std::map<std::string, Bitmap> fontMap;
  parseMagicNumber();
  auto dimensions = parseDimensions();
  std::vector<std::string> letters = parseLetters();
  for (auto& letter : letters) {
    fontMap.insert(std::make_pair(
        letter, parseBitmap(dimensions.first, dimensions.second)));
    nextCharacter();
  }

  return Font(fontMap);
}

std::string FontLoader::parseMagicNumber() {
  ignoreWhitespaces();
  parseComment();
  auto magicNumber = currentCharacter_;
  if (currentCharacter_ == "P1") {
    nextCharacter();
  }
  return magicNumber;
}

Bitmap FontLoader::parseBitmap(size_t dim1, size_t dim2) {
  ignoreWhitespaces();
  parseComment();
  std::vector<bool> bitmap;
  std::string stringBitmap;
  stringBitmap = currentCharacter_;
  std::transform(stringBitmap.begin(), stringBitmap.end(),
                 std::back_inserter(bitmap), [](char& c) {
                   if (c == '0') return false;
                   return true;
                 });
  return Bitmap(dim1, dim2, bitmap);
}

std::pair<size_t, size_t> FontLoader::parseDimensions() {
  ignoreWhitespaces();
  parseComment();
  size_t dim1 = std::stoi(currentCharacter_);
  nextCharacter();
  size_t dim2 = std::stoi(currentCharacter_);
  nextCharacter();
  return std::make_pair(dim1, dim2);
}
std::vector<std::string> FontLoader::parseLetters() {
  ignoreWhitespaces();
  parseComment();
  std::vector<std::string> letters;
  size_t lettersNum = std::stoi(currentCharacter_);
  nextCharacter();
  while (lettersNum != 0) {
    ignoreWhitespaces();
    parseComment();
    if (std::isalnum(currentCharacter_.front())) {
      letters.push_back(currentCharacter_);
      nextCharacter();
      lettersNum--;
    }
  }
  return letters;
}

void FontLoader::parseComment() {
  std::string comment;
  if (currentCharacter_.front() == '#') {
    getline(fileHandle_, comment);
    nextCharacter();
  }
}

void FontLoader::ignoreWhitespaces() {
  while (isspace(currentCharacter_.front())) {
    nextCharacter();
  }
}

}  // namespace EspGL
}  // namespace tamagotchi
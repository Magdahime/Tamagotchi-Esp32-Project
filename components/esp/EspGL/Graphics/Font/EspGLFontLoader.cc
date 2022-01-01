#include "Graphics/Font/EspGLFontLoader.hpp"

#include <ctype.h>

#include <algorithm>
#include <cctype>
#include <iterator>
#include <string>
#include <utility>

#include "Graphics/EspGLBitmap.hpp"
#include "Graphics/Font/EspGLFont.hpp"

namespace tamagotchi {
namespace EspGL {

Font FontLoader::load(std::string filename) {
  std::map<char, Bitmap> fontMap;
  parseMagicNumber();
  auto dimensions = parseDimensions();
  std::vector<char> letters = parseLetters();
  for (auto& letter : letters) {
    fontMap.insert(std::make_pair(
        letter, parseBitmap(dimensions.first, dimensions.second)));
  }

  return Font(fontMap);
}

std::string FontLoader::parseMagicNumber() {
  ignoreWhitespaces();
  parseComment();
  std::string line;
  if (currentCharacter_ == 'P') {
    std::getline(fileHandle_, line);
    return currentCharacter_ + line;
  }

  return "";
}

Bitmap FontLoader::parseBitmap(size_t dim1, size_t dim2) {
  ignoreWhitespaces();
  parseComment();
  std::vector<bool> bitmap;
  std::string stringBitmap;
  std::getline(fileHandle_, stringBitmap);
  stringBitmap = currentCharacter_ + stringBitmap;
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
  std::string dimension;
  std::getline(fileHandle_, dimension, ' ');
  size_t dim1 = std::stoi(dimension);
  std::getline(fileHandle_, dimension);
  size_t dim2 = std::stoi(dimension);
  return std::make_pair(dim1, dim2);
}
std::vector<char> FontLoader::parseLetters() {
  ignoreWhitespaces();
  parseComment();
  std::vector<char> letters;
  std::string line;
  std::getline(fileHandle_, line);
  size_t lettersNum = std::stoi(currentCharacter_ + line);

  while (lettersNum != 0) {
    ignoreWhitespaces();
    parseComment();
    if (std::isalnum(currentCharacter_)) {
      letters.push_back(currentCharacter_);
      nextCharacter();
      lettersNum--;
    }
  }
  return letters;
}

void FontLoader::parseComment() {
  std::string comment;
  if (currentCharacter_ == '#') {
    getline(fileHandle_, comment);
  }
}

void FontLoader::ignoreWhitespaces() {
  while (isspace(currentCharacter_)) {
    nextCharacter();
  }
}

}  // namespace EspGL
}  // namespace tamagotchi
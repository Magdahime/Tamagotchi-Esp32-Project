#include "BitmapLoader.hpp"

namespace tamagotchi {

namespace BitmapLoader {

std::string BitmapLoader::parseMagicNumber() {
  ignoreWhitespaces();
  parseComment();
  auto magicNumber = currentCharacter_;
  if (currentCharacter_.front() != 'P') {
    throw std::runtime_error("Unindentified token at " +
                             std::to_string(characterCounter_) +
                             " - expected magic number!");
  }
  nextCharacter();
  return magicNumber;
}

std::vector<bool> BitmapLoader::parseBitmap(size_t dim1, size_t dim2) {
  ignoreWhitespaces();
  parseComment();
  std::vector<bool> bitmap;
  std::string stringBitmap;
  stringBitmap = currentCharacter_;
  if (stringBitmap.front() != '0' && stringBitmap.front() != '1') {
    throw std::runtime_error("Unindentified token at " +
                             std::to_string(characterCounter_) +
                             " - expected bitmap string!");
  }
  if (stringBitmap.size() != dim1 * dim2) {
    throw std::runtime_error("Bitmap at " + std::to_string(characterCounter_) +
                             " has invalid length!");
  }
  std::transform(stringBitmap.begin(), stringBitmap.end(),
                 std::back_inserter(bitmap), [](char& c) {
                   if (c == '0') return false;
                   return true;
                 });
  nextCharacter();
  return bitmap;
}

int64_t BitmapLoader::parseBitmapNumber() {
  ignoreWhitespaces();
  parseComment();
  size_t lettersNum = std::stoi(currentCharacter_);
  nextCharacter();
  return lettersNum;
}

std::pair<size_t, size_t> BitmapLoader::parseDimensions() {
  ignoreWhitespaces();
  parseComment();
  size_t dim1 = std::stoi(currentCharacter_);
  nextCharacter();
  size_t dim2 = std::stoi(currentCharacter_);
  nextCharacter();
  return std::make_pair(dim1, dim2);
}
std::vector<std::string> BitmapLoader::parseSymbols(int64_t bitmapNum) {
  ignoreWhitespaces();
  parseComment();
  std::vector<std::string> symbols;
  while (bitmapNum != 0) {
    ignoreWhitespaces();
    parseComment();
    if (std::isalnum(currentCharacter_.front()) ||
        ispunct(currentCharacter_.front())) {
      symbols.push_back(currentCharacter_);
      nextCharacter();
      bitmapNum--;
    } else {
      throw std::runtime_error("Unindentified token at " +
                               std::to_string(characterCounter_) +
                               " - expected character!");
    }
  }
  return symbols;
}

void BitmapLoader::parseComment() {
  std::string comment;
  if (currentCharacter_.front() == '#') {
    getline(fileHandle_, comment);
    characterCounter_ += comment.size();
    nextCharacter();
  }
}

void BitmapLoader::ignoreWhitespaces() {
  while (isspace(currentCharacter_.front())) {
    nextCharacter();
  }
}

}  // namespace BitmapLoader
}  // namespace tamagotchi
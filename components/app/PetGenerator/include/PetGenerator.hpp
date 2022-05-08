#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "BitmapLoader.hpp"
#include "ColourProvider.hpp"
#include "EspGLUtils.hpp"
#include "Globals.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "Pet.hpp"
#include "SPIFFSDriver.hpp"
#include "esp_random.h"

namespace tamagotchi {
namespace App {
namespace Pet {

template <typename ColourRepresentation>
class PetGenerator {
 public:
  PetGenerator(std::string filename)
      : bitmapLoader_(tamagotchi::App::Globals::spiffsDriver.getFileDescriptor(
            filename)) {
    loadData();
  }
  Pet<ColourRepresentation> generate();

  inline std::map<std::string, EspGL::Bitmap>& bodies() { return bodies_; }
  inline std::map<std::string, EspGL::Bitmap>& eyes() { return eyes_; }
  inline std::map<std::string, EspGL::Bitmap>& faces() { return faces_; }
  inline EspGL::Bitmap& petBitmap() { return petBitmap_; }

 private:
  BitmapLoader::BitmapLoader bitmapLoader_;
  EspGL::Bitmap petBitmap_;
  std::map<std::string, EspGL::Bitmap> eyes_;
  std::map<std::string, EspGL::Bitmap> faces_;
  std::map<std::string, EspGL::Bitmap> bodies_;

  void parseData(std::map<std::string, EspGL::Bitmap>& bitmapContainer);
  void loadData();
  std::pair<std::string, EspGL::Bitmap> choose(
      std::map<std::string, EspGL::Bitmap>& bodyPart);
  std::pair<std::string, EspGL::Bitmap> chooseBody();
  std::pair<std::string, EspGL::Bitmap> chooseEyes();
  std::pair<std::string, EspGL::Bitmap> chooseFace();
  EspGL::Colour<ColourRepresentation> chooseColour();
};

template <typename ColourRepresentation>
void PetGenerator<ColourRepresentation>::parseData(
    std::map<std::string, EspGL::Bitmap>& bitmapContainer) {
  auto dimensions = bitmapLoader_.parseDimensions();
  auto bitmapNumber = bitmapLoader_.parseBitmapNumber();
  std::vector<std::string> labels = bitmapLoader_.parseSymbols(bitmapNumber);
  for (auto i = 0; i < bitmapNumber; ++i) {
    bitmapContainer[labels[i]] = EspGL::Bitmap(
        dimensions.first, dimensions.second,
        bitmapLoader_.parseBitmap(dimensions.first, dimensions.second));
  }
}

template <typename ColourRepresentation>
void PetGenerator<ColourRepresentation>::loadData() {
  bitmapLoader_.parseMagicNumber();
  parseData(bodies_);
  parseData(faces_);
  parseData(eyes_);
}

template <typename ColourRepresentation>
std::pair<std::string, EspGL::Bitmap>
PetGenerator<ColourRepresentation>::choose(
    std::map<std::string, EspGL::Bitmap>& bodyPart) {
  std::vector<std::string> keys;
  std::transform(
      bodyPart.begin(), bodyPart.end(), std::back_inserter(keys),
      [](const std::map<std::string, EspGL::Bitmap>::value_type& pair) {
        return pair.first;
      });
  auto randomNum = esp_random() % bodyPart.size();
  return std::make_pair(keys[randomNum], bodyPart[keys[randomNum]]);
}

template <typename ColourRepresentation>
std::pair<std::string, EspGL::Bitmap>
PetGenerator<ColourRepresentation>::chooseBody() {
  return choose(bodies_);
}
template <typename ColourRepresentation>
std::pair<std::string, EspGL::Bitmap>
PetGenerator<ColourRepresentation>::chooseEyes() {
  return choose(eyes_);
}

template <typename ColourRepresentation>
std::pair<std::string, EspGL::Bitmap>
PetGenerator<ColourRepresentation>::chooseFace() {
  return choose(faces_);
}

template <typename ColourRepresentation>
EspGL::Colour<ColourRepresentation>
PetGenerator<ColourRepresentation>::chooseColour() {
  auto colours = ColourProvider::getPossiblePetColours<ColourRepresentation>();
  return colours[esp_random() % colours.size()];
}

template <typename ColourRepresentation>
Pet<ColourRepresentation> PetGenerator<ColourRepresentation>::generate() {
  auto petBitmap = chooseBody();
  auto eyes = chooseEyes();
  auto face = chooseFace();
  return Pet<ColourRepresentation>(petBitmap, eyes, face, chooseColour());
}

}  // namespace Pet
}  // namespace App

}  // namespace tamagotchi

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

  inline std::vector<EspGL::Bitmap>& bodies() { return bodies_; }
  inline std::vector<EspGL::Bitmap>& eyes() { return eyes_; }
  inline std::vector<EspGL::Bitmap>& faces() { return faces_; }
  inline EspGL::Bitmap& petBitmap() { return petBitmap_; }

 private:
  BitmapLoader::BitmapLoader bitmapLoader_;
  EspGL::Bitmap petBitmap_;
  std::vector<EspGL::Bitmap> eyes_;
  std::vector<EspGL::Bitmap> faces_;
  std::vector<EspGL::Bitmap> bodies_;

  void parseData(std::vector<EspGL::Bitmap>& bitmapContainer);
  void loadData();
  std::pair<uint8_t, EspGL::Bitmap> choose(
      std::vector<EspGL::Bitmap>& bodyPart);
  std::pair<uint8_t, EspGL::Bitmap> chooseBody();
  std::pair<uint8_t, EspGL::Bitmap> chooseEyes();
  std::pair<uint8_t, EspGL::Bitmap> chooseFace();
  EspGL::Colour<ColourRepresentation> chooseColour();
};

template <typename ColourRepresentation>
void PetGenerator<ColourRepresentation>::parseData(
    std::vector<EspGL::Bitmap>& bitmapContainer) {
  auto dimensions = bitmapLoader_.parseDimensions();
  auto bitmapNumber = bitmapLoader_.parseBitmapNumber();
  bitmapLoader_.parseSymbols(bitmapNumber);
  for (auto i = 0; i < bitmapNumber; ++i) {
    bitmapContainer.push_back(EspGL::Bitmap(
        dimensions.first, dimensions.second,
        bitmapLoader_.parseBitmap(dimensions.first, dimensions.second)));
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
std::pair<uint8_t, EspGL::Bitmap> PetGenerator<ColourRepresentation>::choose(
    std::vector<EspGL::Bitmap>& bodyPart) {
  auto randomNum = esp_random() % bodyPart.size();
  return std::make_pair(randomNum, bodyPart[randomNum]);
}

template <typename ColourRepresentation>
std::pair<uint8_t, EspGL::Bitmap>
PetGenerator<ColourRepresentation>::chooseBody() {
  return choose(bodies_);
}
template <typename ColourRepresentation>
std::pair<uint8_t, EspGL::Bitmap>
PetGenerator<ColourRepresentation>::chooseEyes() {
  return choose(eyes_);
}

template <typename ColourRepresentation>
std::pair<uint8_t, EspGL::Bitmap>
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

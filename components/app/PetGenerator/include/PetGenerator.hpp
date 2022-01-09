#pragma once
#include <vector>

#include "BitmapLoader.hpp"
#include "ColourProvider.hpp"
#include "EspGLUtils.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "Pet.hpp"
#include "SPIFFSDriver.hpp"
#include "esp_random.h"

namespace tamagotchi {
namespace App {
namespace Pet {

constexpr int EYES_COLUMN = 5;
constexpr int EYES_ROW = 10;
constexpr int FACE_COLUMN = 5;
constexpr int FACE_ROW = 13;

template <typename ColourRepresentation>
class PetGenerator {
 public:
  PetGenerator(std::string filename)
      : spiffsDriver_(),
        bitmapLoader_(spiffsDriver_.getFileDescriptor(filename)) {
    loadData();
  }
  Pet<ColourRepresentation> generate();

  inline std::vector<EspGL::Bitmap>& bodies() { return bodies_; }
  inline std::vector<EspGL::Bitmap>& eyes() { return eyes_; }
  inline std::vector<EspGL::Bitmap>& faces() { return faces_; }
  inline EspGL::Bitmap& petBitmap() { return petBitmap_; }

 private:
  SPIFFS::SPIFFSDriver spiffsDriver_;
  BitmapLoader::BitmapLoader bitmapLoader_;
  EspGL::Bitmap petBitmap_;
  std::vector<EspGL::Bitmap> eyes_;
  std::vector<EspGL::Bitmap> faces_;
  std::vector<EspGL::Bitmap> bodies_;

  void parseData(std::vector<EspGL::Bitmap>& bitmapContainer);
  void loadData();
  EspGL::Bitmap chooseBody();
  EspGL::Bitmap chooseEyes();
  EspGL::Bitmap chooseFace();
  EspGL::Colour<ColourRepresentation> chooseColour();
};

template <typename ColourRepresentation>
void PetGenerator<ColourRepresentation>::parseData(
    std::vector<EspGL::Bitmap>& bitmapContainer) {
  auto dimensions = bitmapLoader_.parseDimensions();
  auto bitmapNumber = bitmapLoader_.parseBitmapNumber();
  for (auto i = 0; i < bitmapNumber; ++i) {
    bitmapContainer.emplace_back(
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
EspGL::Bitmap PetGenerator<ColourRepresentation>::chooseBody() {
  return bodies_[esp_random() % bodies_.size()];
}
template <typename ColourRepresentation>
EspGL::Bitmap PetGenerator<ColourRepresentation>::chooseEyes() {
  return eyes_[esp_random() % eyes_.size()];
}

template <typename ColourRepresentation>
EspGL::Bitmap PetGenerator<ColourRepresentation>::chooseFace() {
  return faces_[esp_random() % faces_.size()];
}

template <typename ColourRepresentation>
EspGL::Colour<ColourRepresentation>
PetGenerator<ColourRepresentation>::chooseColour() {
  auto colours = ColourProvider::getPossibleColours<ColourRepresentation>();
  return colours[esp_random() % colours.size()];
}

template <typename ColourRepresentation>
Pet<ColourRepresentation> PetGenerator<ColourRepresentation>::generate() {
  EspGL::Bitmap petBitmap = chooseBody();
  EspGL::Bitmap eyes = chooseEyes();
  EspGL::Bitmap face = chooseFace();
  EspGL::Point eyesPoint(EYES_COLUMN, EYES_ROW);
  EspGL::Point facePoint(FACE_COLUMN, FACE_ROW);
  petBitmap.overlay(eyes, eyesPoint);
  petBitmap.overlay(eyes, facePoint);
  return Pet<ColourRepresentation>(petBitmap, chooseColour());
}

}  // namespace Pet
}  // namespace App

}  // namespace tamagotchi

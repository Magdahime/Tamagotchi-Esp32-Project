#pragma once
#include <vector>

#include "EspGLUtils.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "Pet.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {

template <typename ColourRepresentation>
class PetGenerator {
 public:
  Pet generate();

 private:
  std::vector<EspGL::Bitmap> eyes_;
  std::vector<EspGL::Bitmap> faces_;
  std::vector<EspGL::Bitmap> bodies_;
  void loadData();
  void chooseBody(std::vector<bool>& bitmap);
  void chooseEyes(std::vector<bool>& bitmap);
  void chooseFace(std::vector<bool>& bitmap);
  EspGL::Colour<ColourRepresentation> chooseColour();
};

}  // namespace Pet
}  // namespace App

}  // namespace tamagotchi

#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include "EspGLUtils.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "Graphics/EspGLPicture.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {

template <typename ColourRepresentation>
class Pet {
 public:
  void giveFood();
  void giveSnack();
  void play();
  void playWithFriends();



 private:
  std::vector<uint16_t> needs_;
  EspGL::Colour<ColourRepresentation> colour_;
  EspGL::Bitmap body;
  std::string name_;

};
}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

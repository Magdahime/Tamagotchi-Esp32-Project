#pragma once

#include <stdint.h>

#include <array>
#include <string>

#include "EspGLUtils.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "Graphics/EspGLPicture.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {

constexpr int32_t PET_NEEDS = 2;

template <typename ColourRepresentation>
class Pet {
 public:
  Pet(EspGL::Bitmap body, EspGL::Colour<ColourRepresentation> colour)
      : needs_({100}), colour_(std::move(colour)), body_(std::move(body)) {}
  void giveFood();
  void giveSnack();
  void play();
  void playWithFriends();

  void draw(EspGL::Screen<ColourRepresentation> &target, EspGL::Point start);

 private:
  std::array<uint16_t, PET_NEEDS> needs_;
  EspGL::Colour<ColourRepresentation> colour_;
  EspGL::Bitmap body_;
  std::string name_;
};

template <typename ColourRepresentation>
void Pet<ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation> &target, EspGL::Point start) {
  body_.draw(target, start, colour_);
}

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

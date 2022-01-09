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
constexpr int EYES_OFFSETX = 5;
constexpr int EYES_OFFSETY = 10;
constexpr int FACE_OFFSETY = 13;

template <typename ColourRepresentation>
class Pet {
 public:
  Pet(EspGL::Bitmap body, EspGL::Bitmap eyes, EspGL::Bitmap face,
      EspGL::Colour<ColourRepresentation> colour)
      : needs_({100}),
        colour_(std::move(colour)),
        body_(std::move(body)),
        eyes_(std::move(eyes)),
        face_(std::move(face)) {}
  void giveFood();
  void giveSnack();
  void play();
  void playWithFriends();

  void draw(EspGL::Screen<ColourRepresentation>& target, EspGL::Point start,
            uint32_t scale = 5);

  inline EspGL::Bitmap& body() { return body_; }
  inline EspGL::Bitmap& eyes() { return eyes_; }
  inline EspGL::Bitmap& face() { return face_; }
  inline std::string& name() { return name_; }

 private:
  std::array<uint16_t, PET_NEEDS> needs_;
  EspGL::Colour<ColourRepresentation> colour_;
  EspGL::Bitmap body_;
  EspGL::Bitmap eyes_;
  EspGL::Bitmap face_;
  std::string name_;
};

template <typename ColourRepresentation>
void Pet<ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation>& target, EspGL::Point start,
    uint32_t scale) {
  body_.drawScaled(target, start, colour_, scale);
  EspGL::Point eyesPoint{start.x_ + EYES_OFFSETX * scale,
                         start.y_ + EYES_OFFSETY * scale};
  EspGL::Point facePoint{start.x_ + EYES_OFFSETX * scale,
                         start.y_ + FACE_OFFSETY * scale};
  eyes_.drawScaled(target, eyesPoint, colour_, scale);
  face_.drawScaled(target, facePoint, colour_, scale);
}

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

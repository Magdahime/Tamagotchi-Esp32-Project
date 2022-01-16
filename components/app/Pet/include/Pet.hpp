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
  Pet(std::pair<std::string, EspGL::Bitmap> body, std::pair<std::string, EspGL::Bitmap> eyes, std::pair<std::string, EspGL::Bitmap> face,
      EspGL::Colour<ColourRepresentation> colour)
      : needs_({100}),
        colour_(std::move(colour)),
        body_(std::move(body)),
        eyes_(std::move(eyes)),
        face_(std::move(face)) {}
  void giveFood();
  void giveSnack();
  void play();

  void draw(EspGL::Screen<ColourRepresentation>& target, EspGL::Point start,
            uint32_t scale = 5);

  inline EspGL::Bitmap& body() { return body_.second; }
  inline EspGL::Bitmap& eyes() { return eyes_.second; }
  inline EspGL::Bitmap& face() { return face_.second; }
  inline std::string& name() { return name_; }

 private:
  std::array<uint16_t, PET_NEEDS> needs_;
  EspGL::Colour<ColourRepresentation> colour_;
  std::pair<std::string, EspGL::Bitmap> body_;
  std::pair<std::string, EspGL::Bitmap> eyes_;
  std::pair<std::string, EspGL::Bitmap> face_;
  std::string name_;
};

template <typename ColourRepresentation>
void Pet<ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation>& target, EspGL::Point start,
    uint32_t scale) {
  body_.second.drawScaled(target, start, colour_, scale);
  EspGL::Point eyesPoint{start.x_ + EYES_OFFSETX * scale,
                         start.y_ + EYES_OFFSETY * scale};
  EspGL::Point facePoint{start.x_ + EYES_OFFSETX * scale,
                         start.y_ + FACE_OFFSETY * scale};
  eyes_.second.drawScaled(target, eyesPoint, colour_, scale);
  face_.second.drawScaled(target, facePoint, colour_, scale);
}

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

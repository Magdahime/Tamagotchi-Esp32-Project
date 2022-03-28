#pragma once

#include <stdint.h>

#include <array>
#include <string>

#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "Graphics/EspGLPicture.hpp"

namespace tamagotchi {
namespace App {

namespace Pet {

constexpr size_t PET_NEEDS = 2;
constexpr int MAX_NEED_VALUE = 100;
constexpr int EYES_OFFSETX = 5;
constexpr int EYES_OFFSETY = 10;
constexpr int FACE_OFFSETY = 13;

template <typename ColourRepresentation>
class Pet {
 public:
  Pet(std::pair<std::string, EspGL::Bitmap> body,
      std::pair<std::string, EspGL::Bitmap> eyes,
      std::pair<std::string, EspGL::Bitmap> face,
      EspGL::Colour<ColourRepresentation> colour)
      : name_(),
        colour_(std::move(colour)),
        body_(std::move(body)),
        eyes_(std::move(eyes)),
        face_(std::move(face)) {
    needs_.resize(PET_NEEDS);
    std::fill(needs_.begin(), needs_.end(), MAX_NEED_VALUE);
  }
  Pet() = default;
  virtual ~Pet() = default;

  void giveFood();
  void giveSnack();
  void play();

  void draw(EspGL::Screen<ColourRepresentation>& target, EspGL::Vect2 start,
            uint32_t scale = 5);

  inline std::pair<std::string, EspGL::Bitmap>& body() { return body_; }
  inline std::pair<std::string, EspGL::Bitmap>& eyes() { return eyes_; }
  inline std::pair<std::string, EspGL::Bitmap>& face() { return face_; }
  inline std::string& name() { return name_; }
  inline std::vector<int16_t>& needs() { return needs_; }
  inline EspGL::Colour<ColourRepresentation>& colour() { return colour_; }

 private:
  std::string name_;
  std::vector<int16_t> needs_;

  EspGL::Colour<ColourRepresentation> colour_;
  std::pair<std::string, EspGL::Bitmap> body_;
  std::pair<std::string, EspGL::Bitmap> eyes_;
  std::pair<std::string, EspGL::Bitmap> face_;
};

template <typename ColourRepresentation>
void Pet<ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation>& target, EspGL::Vect2 start,
    uint32_t scale) {
  body_.second.drawScaled(target, start, colour_, scale);
  EspGL::Vect2 eyesPoint{start.x_ + EYES_OFFSETX * scale,
                         start.y_ + EYES_OFFSETY * scale};
  EspGL::Vect2 facePoint{start.x_ + EYES_OFFSETX * scale,
                         start.y_ + FACE_OFFSETY * scale};
  eyes_.second.drawScaled(target, eyesPoint, colour_, scale);
  face_.second.drawScaled(target, facePoint, colour_, scale);
}

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

#pragma once

#include <stdint.h>

#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "PetCommon.hpp"

namespace tamagotchi::App::Pet {

template <typename ColourRepresentation>
class BitmapPet : public EspGL::Drawable<ColourRepresentation> {
 public:
  BitmapPet() = default;
  BitmapPet(std::pair<uint8_t, EspGL::Bitmap> body,
            std::pair<uint8_t, EspGL::Bitmap> eyes,
            std::pair<uint8_t, EspGL::Bitmap> face,
            EspGL::Colour<ColourRepresentation> colour,
            EspGL::Vect2 start = EspGL::Vect2(0, 0), int scale = 5)
      : scale_(scale),
        start_(start),
        colour_(std::move(colour)),
        body_(std::move(body)),
        eyes_(std::move(eyes)),
        face_(std::move(face)) {}

  ~BitmapPet() = default;

  virtual void draw(EspGL::Screen<ColourRepresentation>& target) override {
    BitmapPet<ColourRepresentation>::draw(target, start_, scale_);
  }

  virtual EspGL::Hitbox hitbox() override {
    return std::make_pair(
        EspGL::Vect2(start_.x_, start_.y_),
        EspGL::Vect2(start_.x_ + this->body().second.sizeX() * scale_,
                     start_.y_ + this->body().second.sizeY() * scale_));
  }

  void setStart(EspGL::Vect2 newStart) { start_ = newStart; }
  int& scale() { return scale_; }
  EspGL::Vect2& start() { return start_; }
  inline std::pair<uint8_t, EspGL::Bitmap>& body() { return body_; }
  inline std::pair<uint8_t, EspGL::Bitmap>& eyes() { return eyes_; }
  inline std::pair<uint8_t, EspGL::Bitmap>& face() { return face_; }
  inline EspGL::Colour<ColourRepresentation>& colour() { return colour_; }

  void draw(EspGL::Screen<ColourRepresentation>& target, EspGL::Vect2 start,
            uint32_t scale = 5);

  void setColour(EspGL::Colour<ColourRepresentation> newColour) {
    colour_ = newColour;
  }

 protected:
  int scale_;
  EspGL::Vect2 start_;
  EspGL::Colour<ColourRepresentation> colour_;
  std::pair<uint8_t, EspGL::Bitmap> body_;
  std::pair<uint8_t, EspGL::Bitmap> eyes_;
  std::pair<uint8_t, EspGL::Bitmap> face_;
};

template <typename ColourRepresentation>
void BitmapPet<ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation>& target, EspGL::Vect2 start,
    uint32_t scale) {
  body_.second.drawScaled(target, start, colour_, scale);
  EspGL::Vect2 eyesPoint{start.x_ + consts::EYES_OFFSETX * scale,
                         start.y_ + consts::EYES_OFFSETY * scale};
  EspGL::Vect2 facePoint{start.x_ + consts::EYES_OFFSETX * scale,
                         start.y_ + consts::FACE_OFFSETY * scale};
  eyes_.second.drawScaled(target, eyesPoint, colour_, scale);
  face_.second.drawScaled(target, facePoint, colour_, scale);
}

}  // namespace tamagotchi::App::Pet
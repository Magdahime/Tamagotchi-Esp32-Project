#pragma once

#include <stdint.h>

#include <utility>

#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Pet.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {

template <typename ColourRepresentation>
class DrawablePet : public Pet<ColourRepresentation>,
                    public EspGL::Drawable<ColourRepresentation> {
 public:
  DrawablePet() = default;
  DrawablePet(std::pair<std::string, EspGL::Bitmap> body,
              std::pair<std::string, EspGL::Bitmap> eyes,
              std::pair<std::string, EspGL::Bitmap> face,
              EspGL::Colour<ColourRepresentation> colour, EspGL::Vect2 start,
              int scale = 5)
      : Pet<ColourRepresentation>(body, eyes, face, colour),
        scale_(scale),
        start_(start) {}

  DrawablePet(Pet<ColourRepresentation> pet, EspGL::Vect2 start, int scale = 5)
      : Pet<ColourRepresentation>(std::move(pet)),
        scale_(scale),
        start_(start) {}

  ~DrawablePet() = default;

  int& scale() { return scale_; }
  EspGL::Vect2& start() { return start_; }

  void setStart(EspGL::Vect2 newStart) { start_ = newStart; }

  virtual void draw(EspGL::Screen<ColourRepresentation>& target) override {
    Pet<ColourRepresentation>::draw(target, start_, scale_);
  }
  virtual std::pair<EspGL::Vect2, EspGL::Vect2> hitbox() override {
    return std::make_pair(
        EspGL::Vect2(start_.x_, start_.y_),
        EspGL::Vect2(start_.x_ + this->body().second.sizeX() * scale_,
                     start_.y_ + this->body().second.sizeY() * scale_));
  }

 private:
  int scale_;
  EspGL::Vect2 start_;
};

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

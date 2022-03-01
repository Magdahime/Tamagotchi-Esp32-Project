#pragma once
#include <stdint.h>

#include <vector>

#include "Drawable.hpp"
#include "EspGLScreen.hpp"
#include "Graphics/EspGLPicture.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Picture : Drawable<ColourRepresentation> {
 public:
  Picture(Point start, size_t sizeX, size_t sizeY,
          std::vector<ColourRepresentation> picture)
      : start_(start),
        sizeX_(std::move(sizeX)),
        sizeY_(std::move(sizeY)),
        picture_(std::move(picture)) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;
  inline size_t sizeX() { return sizeX_; }
  inline size_t sizeY() { return sizeY_; }
  inline std::vector<ColourRepresentation>& picture() { return picture_; }

 private:
  Point start_;
  size_t sizeX_;
  size_t sizeY_;
  std::vector<ColourRepresentation> picture_;
};

template <typename ColourRepresentation>
void Picture<ColourRepresentation>::draw(Screen<ColourRepresentation>& target) {
  target.screenDriver()->writePixelArea(
      start_.x_, start_.x_ + sizeX_, start_.y_, start_.y_ + sizeY_, picture_);
}

}  // namespace EspGL
}  // namespace tamagotchi
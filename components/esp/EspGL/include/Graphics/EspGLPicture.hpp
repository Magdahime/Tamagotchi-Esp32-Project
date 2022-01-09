#pragma once
#include <stdint.h>

#include <vector>

#include "EspGLScreen.hpp"
#include "Graphics/EspGLPicture.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Picture {
 public:
  Picture(size_t sizeX, size_t sizeY, std::vector<ColourRepresentation> picture)
      : sizeX_(std::move(sizeX)),
        sizeY_(std::move(sizeY)),
        picture_(std::move(picture)) {}
  void draw(Screen<ColourRepresentation>& target, const Point& start);
  inline size_t sizeX() { return sizeX_; }
  inline size_t sizeY() { return sizeY_; }
  inline std::vector<ColourRepresentation>& picture() { return picture_; }

 private:
  size_t sizeX_;
  size_t sizeY_;
  std::vector<ColourRepresentation> picture_;
};

template <typename ColourRepresentation>
void Picture<ColourRepresentation>::draw(Screen<ColourRepresentation>& target,
                                         const Point& start) {
  target.screenDriver()->writePixelArea(picture_);
}

}  // namespace EspGL
}  // namespace tamagotchi
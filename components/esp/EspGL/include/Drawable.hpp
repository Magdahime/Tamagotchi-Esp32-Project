#pragma once

#include <utility>

#include "EspGLScreen.hpp"
#include "EspGLUtils.hpp"
namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Drawable {
 public:
  virtual void draw(Screen<ColourRepresentation>& target) = 0;
  void redraw(Screen<ColourRepresentation>& target);

  void erase(Screen<ColourRepresentation>& target,
             Colour<ColourRepresentation> backgroundColour);
  virtual std::pair<Vect2, Vect2> hitbox() = 0;

  virtual ~Drawable() = default;

 private:
};

template <typename ColourRepresentation>
void Drawable<ColourRepresentation>::redraw(
    Screen<ColourRepresentation>& target) {
  draw(target);
}

template <typename ColourRepresentation>
void Drawable<ColourRepresentation>::erase(
    Screen<ColourRepresentation>& target,
    Colour<ColourRepresentation> backgroundColour) {
  auto drawableHitbox = hitbox();
  target.screenDriver()->writePixelArea(
      drawableHitbox.first.x_, drawableHitbox.second.x_,
      drawableHitbox.first.y_, drawableHitbox.second.y_,
      backgroundColour.value());
}

}  // namespace EspGL
}  // namespace tamagotchi
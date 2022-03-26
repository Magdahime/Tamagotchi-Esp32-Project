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
  void redraw(Screen<ColourRepresentation>& target,
              Colour<ColourRepresentation> backgroundColour);
  virtual std::pair<Point, Point> getHitbox() = 0;

  virtual ~Drawable() = default;

 private:
};

template <typename ColourRepresentation>
void Drawable<ColourRepresentation>::redraw(
    Screen<ColourRepresentation>& target,
    Colour<ColourRepresentation> backgroundColour) {
  auto hitbox = getHitbox();
  target.screenDriver()->writePixelArea(hitbox.first.x_, hitbox.second.x_,
                                        hitbox.first.y_, hitbox.second.y_,
                                        backgroundColour.value());
  draw(target);
}

}  // namespace EspGL
}  // namespace tamagotchi
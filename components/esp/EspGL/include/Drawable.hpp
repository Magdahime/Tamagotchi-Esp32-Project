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
  virtual std::pair<Point, Point> getHitbox() = 0;

 private:
};

}  // namespace EspGL
}  // namespace tamagotchi
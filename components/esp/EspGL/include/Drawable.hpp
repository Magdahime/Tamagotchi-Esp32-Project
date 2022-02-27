#pragma once

#include "EspGLScreen.hpp"
namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Drawable {
 public:
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

 private:
};

}  // namespace EspGL
}  // namespace tamagotchi
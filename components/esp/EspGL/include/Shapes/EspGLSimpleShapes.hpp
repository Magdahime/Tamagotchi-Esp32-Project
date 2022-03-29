#pragma once

#include <stdint.h>

#include <array>
#include <optional>
#include <stdexcept>
#include <utility>

#include "Drawable.hpp"
#include "EspGL.hpp"
#include "EspGLScreen.hpp"
#include "EspGLUtils.hpp"

namespace tamagotchi {

namespace EspGL {

template <typename ColourRepresentation>
class Shape : Drawable<ColourRepresentation> {
 public:
  virtual ~Shape() = default;

 protected:
};

template <typename ColourRepresentation>
class Line : public Shape<ColourRepresentation> {
 public:
  Line(Vect2 point1, Vect2 point2, Colour<ColourRepresentation> colour)
      : start_(std::move(point1)),
        end_(std::move(point2)),
        colour_(std::move(colour)) {}
  virtual ~Line() = default;
  virtual void draw(Screen<ColourRepresentation> &target) override;

  inline const Vect2 &start() const { return start_; }
  inline const Vect2 &end() const { return end_; }
  inline const Colour<ColourRepresentation> &colour() const { return colour_; }

  inline void setStart(Vect2 newStart) { start_ = newStart; }
  inline void setEnd(Vect2 newEnd) { end_ = newEnd; }
  inline void setColour(Colour<ColourRepresentation> newColour) {
    colour_ = newColour;
  }

  virtual inline Hitbox hitbox() override {
    return std::make_pair(start_, end_);
  }

 private:
  Vect2 start_;
  Vect2 end_;
  Colour<ColourRepresentation> colour_;
};

template <typename ColourRepresentation>
void Line<ColourRepresentation>::draw(Screen<ColourRepresentation> &target) {
  ESP_LOGD(TAG_,
           "Drawing line: (%d,%d) -> (%d,%d) in "
           "colour: 0x%X",
           start_.x_, start_.y_, end_.x_, end_.y_, colour_.value());
  int16_t kx = end_.x_ >= start_.x_ ? 1 : -1;
  int16_t ky = end_.y_ >= start_.y_ ? 1 : -1;
  int16_t dx = abs(end_.x_ - start_.x_);
  int16_t dy = abs(end_.y_ - start_.y_);
  target.screenDriver()->drawPixel(start_, colour_.value());

  int16_t currX = start_.x_;
  int16_t currY = start_.y_;
  auto bresenhamLoop = [&](int16_t &v1, int16_t &v2, int16_t &k1, int16_t &k2,
                           int16_t &d1, int16_t &d2) {
    double err = d1 / 2.0;
    for (auto i = 0; i < d1; ++i) {
      v1 = v1 + k1;
      err = err - d2;
      if (err < 0) {
        v2 = v2 + k2;
        err = err + d1;
      }
      target.screenDriver()->drawPixel({currX, currY}, colour_.value());
    }
  };
  if (dx >= dy) {
    bresenhamLoop(currX, currY, kx, ky, dx, dy);
  } else {
    bresenhamLoop(currY, currX, ky, kx, dy, dx);
  }
}

}  // namespace EspGL

}  // namespace tamagotchi
#pragma once

#include <stdint.h>

#include "EspGL.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"
#include "EspGlScreen.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class TriangleBase : public Shape<ColourRepresentation> {
 public:
  TriangleBase(Point point1, Point point2, Point point3)
      : point1_(std::move(point1)),
        point2_(std::move(point2)),
        point3_(std::move(point3)) {}

  virtual void draw(Screen<ColourRepresentation>& target) = 0;

 protected:
  Point point1_;
  Point point2_;
  Point point3_;
};

template <typename ColourRepresentation>
class Triangle : public TriangleBase<ColourRepresentation> {
 public:
  Triangle(Point point1, Point point2, Point point3,
           Colour<ColourRepresentation> fill)
      : TriangleBase<ColourRepresentation>(point1, point2, point3),
        fill_(std::move(fill)) {}
  Triangle(Point point1, Point point2, Point point3,
           Colour<ColourRepresentation> fill,
           Colour<ColourRepresentation> outline)
      : TriangleBase<ColourRepresentation>(point1, point2, point3),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

template <typename ColourRepresentation>
class TriangleOutline : public TriangleBase<ColourRepresentation> {
 public:
  TriangleOutline(Point point1, Point point2, Point point3,
                  Colour<ColourRepresentation> outline)
      : TriangleBase<ColourRepresentation>(point1, point2, point3),
        outline_(std::move(outline)) {}

  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation>
class EquilateralTriangle : public Triangle<ColourRepresentation> {
 public:
  EquilateralTriangle(Point point1, Point point2, Point point3,
                      Colour<ColourRepresentation> fill);
  EquilateralTriangle(Point point1, Point point2, Point point3,
                      Colour<ColourRepresentation> fill,
                      Colour<ColourRepresentation> outline);
  EquilateralTriangle(Point center, double sideLength,
                      Colour<ColourRepresentation> fill, double angle = 0);
  EquilateralTriangle(Point center, double sideLength,
                      Colour<ColourRepresentation> fill,
                      Colour<ColourRepresentation> outline, double angle = 0);
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
  double sideLength_;
  Point center_;
  double angle_;
};
template <typename ColourRepresentation>
class EquilateralTriangleOutline
    : public TriangleOutline<ColourRepresentation> {
 public:
  EquilateralTriangleOutline(Point point1, Point point2, Point point3,
                             Colour<ColourRepresentation> outline);
  EquilateralTriangleOutline(Point center, double sideLength,
                             Colour<ColourRepresentation> outline,
                             double angle = 0);
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
  double sideLength_;
  Point center_;
  float angle_;
};

/////////////////////////////
///       TRIANGLE        ///
/////////////////////////////

template <typename ColourRepresentation>
void Triangle<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  auto [max, mid, min] =
      sort3Points(this->point1_, this->point2_, this->point3_, Coordinate::Y);
  for (auto y = min.y_; y <= mid.y_; ++y) {
    auto point1 = straightLineEquation(min, mid, {{}, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {{}, y}, Coordinate::X);
    Line{point1, point2, fill_}.draw(target);
  }
  for (auto y = mid.y_; y <= max.y_; ++y) {
    auto point1 = straightLineEquation(mid, max, {{}, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {{}, y}, Coordinate::X);
    Line{point1, point2, fill_}.draw(target);
  }
}

template <typename ColourRepresentation>
void TriangleOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  ESP_LOGD(TAG_,
           "Drawing triangle of vertices: (%d,%d) (%d,%d) (%d,%d) in "
           "colour: 0x%X",
           this->point1_.x_, this->point1_.y_, this->point2_.x_,
           this->point2_.y_, this->point3_.x_, this->point3_.y_,
           this->outline_.value());
  Line{this->point1_, this->point2_, this->outline_}.draw(target);
  Line{this->point2_, this->point3_, this->outline_};
  Line{this->point3_, this->point1_, this->outline_};
}

/////////////////////////////
/// EQUILATERAL  TRIANGLE ///
/////////////////////////////
template <typename ColourRepresentation>
void EquilateralTriangle<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  Triangle<ColourRepresentation>::draw(target);
}

template <typename ColourRepresentation>
void EquilateralTriangleOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  TriangleOutline<ColourRepresentation>::draw(target);
}

}  // namespace EspGL
}  // namespace tamagotchi

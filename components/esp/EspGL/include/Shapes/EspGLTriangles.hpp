#pragma once

#include <stdint.h>

#include <cmath>
#include <optional>
#include <stdexcept>

#include "EspGL.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"
#include "EspGlScreen.hpp"
#include "Shapes/EspGLTriangles.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class TriangleBase : public Shape<ColourRepresentation> {
 public:
  TriangleBase(Point point1, Point point2, Point point3)
      : point1_(std::move(point1)),
        point2_(std::move(point2)),
        point3_(std::move(point3)) {}
  virtual ~TriangleBase() = default;
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

  inline const Point& point1() const { return point1_; }
  inline const Point& point2() const { return point2_; }
  inline const Point& point3() const { return point3_; }

  inline void setPoint1(Point newPoint) { point1_ = newPoint; }
  inline void setPoint2(Point newPoint) { point2_ = newPoint; }
  inline void setPoint3(Point newPoint) { point3_ = newPoint; }

  virtual inline std::pair<Point, Point> getHitbox() override {
    return std::make_pair(
        Point{std::min({point1_.x_, point2_.x_, point3_.x_}),
              std::min({point1_.y_, point2_.y_, point3_.y_})},
        Point{std::max({point1_.x_, point2_.x_, point3_.x_}),
              std::max({point1_.y_, point2_.y_, point3_.y_})});
  }

 protected:
  explicit TriangleBase() = default;
  Point point1_;
  Point point2_;
  Point point3_;
};

template <typename ColourRepresentation>
class Triangle : public TriangleBase<ColourRepresentation> {
 public:
  Triangle(Point point1, Point point2, Point point3,
           Colour<ColourRepresentation> fill,
           std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : TriangleBase<ColourRepresentation>(point1, point2, point3),
        fill_(std::move(fill)),
        outline_(outline) {}
  virtual ~Triangle() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline const Colour<ColourRepresentation>& fill() const { return fill_; }
  inline const std::optional<Colour<ColourRepresentation>>& outline() const {
    return outline_.value();
  }

  inline void setFill(Colour<ColourRepresentation> newFill) { fill_ = newFill; }

  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 protected:
  explicit Triangle() = default;
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
  virtual ~TriangleOutline() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline const std::optional<Colour<ColourRepresentation>>& outline() const {
    return outline_.value();
  }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 protected:
  explicit TriangleOutline() = default;
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation>
class EquilateralTriangle : public Triangle<ColourRepresentation> {
 public:
  EquilateralTriangle(
      Point center, double sideLength, Colour<ColourRepresentation> fill,
      std::optional<Colour<ColourRepresentation>> outline = std::nullopt,
      double angle = 0)
      : Triangle<ColourRepresentation>() {
    double radius = std::sqrt(3) * sideLength / 3;
    double radians = M_PI * 2.0 / 3.0;
    Point point1{center.x_ + cos(angle) * radius,
                 center.y_ + sin(angle) * radius};
    Point point2{center.x_ + cos(angle + radians) * radius,
                 center.y_ + sin(angle + radians) * radius};
    Point point3{center.x_ + cos(angle + 2 * radians) * radius,
                 center.y_ + sin(angle + 2 * radians) * radius};
    this->point1_ = point1;
    this->point2_ = point2;
    this->point3_ = point3;
    this->fill_ = fill;
    this->outline_ = outline;
  }
  virtual ~EquilateralTriangle() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
  double sideLength_;
  Point center_;
  double angle_;
};
template <typename ColourRepresentation>
class EquilateralTriangleOutline
    : public TriangleOutline<ColourRepresentation> {
 public:
  EquilateralTriangleOutline(Point center, double sideLength,
                             Colour<ColourRepresentation> outline,
                             double angle = 0)
      : TriangleOutline<ColourRepresentation>() {
    double radius = std::sqrt(3) * sideLength / 3;
    double radians = M_PI * 2.0 / 3.0;
    Point point1{center.x_ + cos(angle) * radius,
                 center.y_ + sin(angle) * radius};
    Point point2{center.x_ + cos(angle + radians) * radius,
                 center.y_ + sin(angle + radians) * radius};
    Point point3{center.x_ + cos(angle + 2 * radians) * radius,
                 center.y_ + sin(angle + 2 * radians) * radius};
    this->point1_ = point1;
    this->point2_ = point2;
    this->point3_ = point3;
    this->outline_ = outline;
  }
  virtual ~EquilateralTriangleOutline() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
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
  ESP_LOGD(TAG_,
           "Drawing filled triangle of vertices: (%d,%d) (%d,%d) (%d,%d) in "
           "colour: 0x%X",
           this->point1_.x_, this->point1_.y_, this->point2_.x_,
           this->point2_.y_, this->point3_.x_, this->point3_.y_,
           this->fill_.value());
  auto [max, mid, min] =
      sort3Points(this->point1_, this->point2_, this->point3_, Coordinate::Y);
  for (auto y = min.y_; y <= mid.y_; ++y) {
    auto point1 = straightLineEquation(min, mid, {0, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {0, y}, Coordinate::X);
    Line{point1, point2, fill_}.draw(target);
  }
  for (auto y = mid.y_; y <= max.y_; ++y) {
    auto point1 = straightLineEquation(mid, max, {0, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {0, y}, Coordinate::X);
    Line{point1, point2, fill_}.draw(target);
  }

  if (outline_) {
    TriangleOutline<ColourRepresentation>{this->point1_, this->point2_,
                                          this->point3_, this->outline_.value()}
        .draw(target);
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
  Line{this->point2_, this->point3_, this->outline_}.draw(target);
  Line{this->point3_, this->point1_, this->outline_}.draw(target);
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

#pragma once
#include <array>
#include <cmath>
#include <optional>

#include "EspGL.hpp"
#include "EspGLScreen.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"
#include "Shapes/EspGLPolygons.hpp"
#include "Shapes/EspGLTriangles.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation, unsigned Vertices>
class PolygonBase : public Shape<ColourRepresentation> {
 public:
  PolygonBase(std::array<Point, Vertices> vertices)
      : vertices_(std::move(vertices)) {}
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

  inline const std::array<Point, Vertices>& vertices() const {
    return vertices_;
  }
  inline void setVertices(std::array<Point, Vertices>& newVertices) {
    vertices_ = newVertices;
  }

 protected:
  void drawPolygon(Colour<ColourRepresentation> colour,
                   Screen<ColourRepresentation>& target) {
    ESP_LOGD(TAG_,
             "Drawing polygon of %d "
             "vertices in "
             "colour: 0x%X",
             Vertices, colour.value());
    Point lastPoint = vertices_[0];
    for (auto i = 1; i <= Vertices; i++) {
      Point currentPoint = vertices_[i % Vertices];
      Line<ColourRepresentation>{lastPoint, currentPoint, colour}.draw(target);
      std::swap(lastPoint, currentPoint);
    }
  }

  explicit PolygonBase() = default;
  std::array<Point, Vertices> vertices_;
};

template <typename ColourRepresentation, unsigned Vertices>
class RegularPolygonBase : public PolygonBase<ColourRepresentation, Vertices> {
 public:
  RegularPolygonBase(Point center, double radius, double rotation = 0.0)
      : center_(std::move(center)),
        radius_(std::move(radius)),
        rotation_(std::move(rotation)) {
    std::array<Point, Vertices> vertices;
    for (auto i = 0ll; i < Vertices; i++) {
      vertices[i] = {
          radius_ * cos(2 * M_PI * i / Vertices + rotation_) + center_.x_,
          radius_ * sin(2 * M_PI * i / Vertices + rotation_) + center_.y_};
    }
    this->vertices_ = vertices;
  };

  inline const Point& center() const { return center_; }
  inline double radius() const { return radius_; }
  inline double rotation() const { return rotation_; }

  inline void setCenter(Point newCenter) { center_ = newCenter; }
  inline void setRadius(double newRadius) { radius_ = newRadius; }
  inline void setRotation(double newRotation) { rotation_ = newRotation; }

 protected:
  Point center_;
  double radius_;
  double rotation_;
};

template <typename ColourRepresentation, unsigned Vertices>
class RegularPolygonOutline
    : public RegularPolygonBase<ColourRepresentation, Vertices> {
 public:
  RegularPolygonOutline(Point center, double radius,
                        Colour<ColourRepresentation> outline,
                        double rotation = 0.0)
      : RegularPolygonBase<ColourRepresentation, Vertices>(center, radius,
                                                           rotation),
        outline_(outline) {}

  virtual void draw(Screen<ColourRepresentation>& target) override {
    PolygonBase<ColourRepresentation, Vertices>::drawPolygon(outline_, target);
  }

  inline const Colour<ColourRepresentation>& outline() const  { return outline_; }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 protected:
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation, unsigned Vertices>
class RegularPolygon
    : public RegularPolygonBase<ColourRepresentation, Vertices> {
 public:
  RegularPolygon(
      Point center, uint16_t radius, Colour<ColourRepresentation> fill,
      std::optional<Colour<ColourRepresentation>> outline = std::nullopt,
      double rotation = 0.0)
      : RegularPolygonBase<ColourRepresentation, Vertices>(center, radius,
                                                           rotation),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}

  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline const Colour<ColourRepresentation>& outline() const { return outline_; }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

  inline const Colour<ColourRepresentation>& fill() const { return fill_; }
  inline void setFill(Colour<ColourRepresentation> newFill) { fill_ = newFill; }

 protected:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

/////////////////////////////
///   REGULAR POLYGON     ///
/////////////////////////////

template <typename ColourRepresentation, unsigned Vertices>
void RegularPolygon<ColourRepresentation, Vertices>::draw(
    Screen<ColourRepresentation>& target) {
  ESP_LOGD(TAG_,
           "Drawing filled polygon of center (%d,%d) and radius %f and %d "
           "vertices in "
           "colour: 0x%X",
           this->center_.x_, this->center_.y_, this->radius_, Vertices,
           fill_.value());
  Point lastPoint = this->vertices_[0];
  for (auto i = 1; i <= Vertices; ++i) {
    Point currentPoint = this->vertices_[i % Vertices];
    Triangle<ColourRepresentation>{lastPoint, currentPoint, this->center_,
                                   this->fill_}
        .draw(target);
    std::swap(lastPoint, currentPoint);
  }

  if (outline_) {
    RegularPolygonOutline<ColourRepresentation, Vertices>(
        this->center_, this->radius_, this->outline_.value(), this->rotation_)
        .draw(target);
  }
}

}  // namespace EspGL
}  // namespace tamagotchi

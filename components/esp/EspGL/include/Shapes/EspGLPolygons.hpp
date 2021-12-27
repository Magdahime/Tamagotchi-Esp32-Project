#pragma once
#include <array>
#include <cmath>

#include "EspGL.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"
#include "EspGLScreen.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation, unsigned Vertices>
class Polygon : public Shape<ColourRepresentation> {
 public:
  Polygon(std::array<Point, Vertices> vertices,
          Colour<ColourRepresentation> outline)
      : vertices_(std::move(vertices)), outline_(outline) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
  std::array<Point, Vertices> vertices_;
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation, unsigned Vertices>
class ConvexPolygon : public Polygon<ColourRepresentation, Vertices> {
 public:
  ConvexPolygon(std::array<Point, Vertices> vertices,
                Colour<ColourRepresentation> outline)
      : Polygon<ColourRepresentation, Vertices>(std::move(vertices), outline) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
};

template <typename ColourRepresentation, unsigned Vertices>
class RegularPolygon : public Polygon<ColourRepresentation, Vertices> {
 public:
  RegularPolygon(Point center, uint16_t radius,
                 Colour<ColourRepresentation> outline, double rotation = 0.0)
      : center_(std::move(center)),
        radius_(std::move(radius)),
        rotation_(std::move(rotation)) {
    std::array<Point, Vertices> vertices;
    for (auto i = 0ll; i < Vertices; i++) {
      vertices[i] = {
          radius_ * cos(2 * M_PI * i / Vertices + rotation_) + center_.x_,
          radius_ * sin(2 * M_PI * i / Vertices + rotation_) + center_.y_};
    }
    Polygon<ColourRepresentation, Vertices>::Polygon(vertices, outline);
  };
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
  Point center_;
  uint16_t radius_;
  double rotation_;
};

// /////////////////////////////
// ///       POLYGON         ///
// /////////////////////////////

// template <typename ColourRepresentation, unsigned Vertices>
// void Polygon<ColourRepresentation, Vertices>::draw(
//     Screen<ColourRepresentation>& target) {
//   ESP_LOGD(TAG_,
//            "Drawing polygon of center (%d,%d) and radius %f and %d vertices
//            in " "colour: 0x%X", center.x_, center.y_, r, vertices, colour);
//   double gamma = 2.0 * consts::PI / vertices;
//   EspGL::Point lastPoint{center.x_ + sin(rotation) * r,
//                          center.y_ + cos(rotation) * r};
//   for (auto i = 1; i <= vertices; ++i) {
//     EspGL::Point currentPoint{center.x_ + sin(rotation + i * gamma) * r,
//                               center.y_ + cos(rotation + i * gamma) * r};
//     drawLine(lastPoint, currentPoint, colour);
//     std::swap(lastPoint, currentPoint);
//   }
// }

// void ST7789VWDriver::drawFilledPolygon(const EspGL::Point& center, double r,
//                                        uint16_t vertices, uint16_t colour,
//                                        double rotation) {
//   ESP_LOGD(TAG_,
//            "Drawing filled polygon of center (%d,%d) and radius %f and %d "
//            "vertices in "
//            "colour: 0x%X",
//            center.x_, center.y_, r, vertices, colour);
//   double gamma = 2.0 * consts::PI / vertices;
//   EspGL::Point lastPoint{center.x_ + sin(rotation) * r,
//                          center.y_ + cos(rotation) * r};
//   for (auto i = 1; i <= vertices; ++i) {
//     EspGL::Point currentPoint{center.x_ + sin(rotation + i * gamma) * r,
//                               center.y_ + cos(rotation + i * gamma) * r};
//     drawFilledTriangle(lastPoint, currentPoint, center, colour);
//     std::swap(lastPoint, currentPoint);
//   }
// }

}  // namespace EspGL
}  // namespace tamagotchi

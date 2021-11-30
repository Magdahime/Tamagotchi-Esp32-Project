#pragma once

#include <stdint.h>

#include <array>

#include "EspGLShapes.hpp"
#include "EspGLUtils.hpp"
#include "ST7789Utils.hpp"

namespace tamagotchi {

namespace EspGL {

using Point = tamagotchi::ST7789::Point;

class Shape {
 public:
  virtual void draw() = 0;

 protected:
};

class Line : public Shape {
 public:
  Line(Point point1, Point point2, Colour colour)
      : start(std::move(point1)),
        end(std::move(point2)),
        colour_(std::move(colour)) {}

  virtual void draw() override;

 private:
  Point start_;
  Point end_;
  Colour colour_;
};

class TriangleBase : public Shape {
 public:
  TriangleBase(Point point1, Point point2, Point point3)
      : point1_(std::move(point1)),
        point2_(std::move(point2)),
        point3_(std::move(point3)) {}

  virtual void draw() = 0;

 protected:
  Point point1_;
  Point point2_;
  Point point3_;
};

class Triangle : public TriangleBase {
 public:
  Triangle(Point point1, Point point2, Point point3, Colour fill)
      : TriangleBase(point1, point2, point3), fill_(std::move(fill)) {}

  virtual void draw() override;

 private:
  Colour fill_;
};

class TriangleOutline : public TriangleBase {
 public:
  TriangleOutline(Point point1, Point point2, Point point3, Colour outline)
      : TriangleBase(point1, point2, point3) : outline_(std::move(outline)) {}

  virtual void draw() override;

 private:
  Colour outline_;
};

class EquilateralTriangle : public Triangle {
 public:
  EquilateralTriangle(Point point1, Point point2, Point point3, Colour fill);
  EquilateralTriangle(Point center, Point sideLength, double angle = 0);
  virtual void draw() override;

 private:
  Point sideLength_;
  Point center_;
  double angle_;
};

class EquilateralTriangleOutline : public TriangleOutline {
 public:
  EquilateralTriangleOutline(Point point1, Point point2, Point point3,
                             Colour outline);
  EquilateralTriangleOutline(Point center, Point sideLength, double angle = 0);
  virtual void draw() override;

 private:
  Point sideLength_;
  Point center_;
  float angle_;
};

class RectangleBase : public Shape {
 public:
  RectangleBase(Point leftUpperPoint, uint16_t dimensionX, uint16_t dimensionY);

 protected:
};
class Rectangle : public RectangleBase {};
class RectangleOutline : public RectangleBase {};

class Square : public Rectangle {};
class SquareOutline : public RectangleOutline {};

class CircleBase : public Shape {};
class Circle : public CircleBase {};
class CircleOutline : public CircleBase {};

template <unsigned Vertices>
class Polygon : public Shape {
 public:
  Polygon(std::array<Point, Vertices> vertices)
      : vertices_(std::move(vertices)) {}
  virtual void draw() override;

 protected:
  std::array<Point, Vertices> vertices_;
};

template <unsigned Vertices>
class ConvexPolygon : public Polygon {
 public:
  ConvexPolygon(std::array<Point, Vertices> vertices)
      : Polygon(std::move(vertices)) {}
  virtual void draw() override;

 protected:
};

template <unsigned Vertices>
class RegularPolygon : public Polygon {
 public:
  RegularPolygon(Point center, uint16_t sideLength, double rotation = 0.0)
      : center_(std::move(center)),
        sideLength_(std::move(sideLength_)),
        rotation_(std::move(rotation));
  virtual void draw() override;

 protected:
  Point center_;
  uint16_t sideLength_;
  double rotation_;
};

}  // namespace EspGL

}  // namespace tamagotchi
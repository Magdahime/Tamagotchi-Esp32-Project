#pragma once

#include <stdint.h>

#include <array>
#include <optional>

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
  Triangle(Point point1, Point point2, Point point3, Colour fill,
           Colour outline = NULL)
      : TriangleBase(point1, point2, point3),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}

  virtual void draw() override;

 private:
  Colour fill_;
  std::optional<Colour> outline_;
};

class TriangleOutline : public TriangleBase {
 public:
  TriangleOutline(Point point1, Point point2, Point point3, Colour outline)
      : TriangleBase(point1, point2, point3), outline_(std::move(outline)) {}

  virtual void draw() override;

 private:
  Colour outline_;
};

class EquilateralTriangle : public Triangle {
 public:
  EquilateralTriangle(Point point1, Point point2, Point point3, Colour fill,
                      Colour outline = NULL);
  EquilateralTriangle(Point center, double sideLength, double angle = 0,
                      Colour fill, Colour outline = NULL);
  virtual void draw() override;

 private:
  double sideLength_;
  Point center_;
  double angle_;
};

class EquilateralTriangleOutline : public TriangleOutline {
 public:
  EquilateralTriangleOutline(Point point1, Point point2, Point point3,
                             Colour outline);
  EquilateralTriangleOutline(Point center, double sideLength, double angle = 0);
  virtual void draw() override;

 private:
  double sideLength_;
  Point center_;
  float angle_;
};

class RectangleBase : public Shape {
 public:
  RectangleBase(Point leftUpperPoint, double dimensionX, double dimensionY)
      : leftUpperPoint_(std::move(leftUpperPoint)),
        dimensionX_(dimensionX),
        dimensionY_(dimensionY) {}
  virtual void draw() = 0;

 protected:
  Point leftUpperPoint_;
  double dimensionX_;
  double dimensionY_;
};
class Rectangle : public RectangleBase {
  Rectangle(Point leftUpperPoint, double dimensionX, double dimensionY,
            Colour fill, Colour outline = NULL);
  virtual void draw() override;

 private:
  Colour fill_;
  std::optional<Colour> outline_;
};
class RectangleOutline : public RectangleBase {
 public:
  RectangleOutline(Point leftUpperPoint, double dimensionX, double dimensionY,
                   Colour outline);
  virtual void draw() override;

 private:
  Colour outline_;
};

class Square : public Rectangle {
 public:
  Square(Point leftUpperPoint, double dimensionX, double dimensionY,
         Colour fill, Colour outline = NULL);
  Square(Point center, double sideLength, Colour fill, Colour outline = NULL);
  virtual void draw() override;

 private:
};

class SquareOutline : public RectangleOutline {
 public:
  Square(Point leftUpperPoint, double dimensionX, double dimensionY,
         Colour outline);
  Square(Point center, double sideLength, Colour outline);
  virtual void draw() override;

 private:
};

class CircleBase : public Shape {
 public:
  CircleBase(Point center, double radius)
      : center_(std::move(center)), radius_(radius) {}
  virtual void draw() = 0;

 protected:
  Point center_;
  double radius_;
};
class Circle : public CircleBase {
 public:
  Circle(Point center, double radius, Colour fill, Colour outline = NULL)
      : CircleBase(center, radius),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}
  virtual void draw() override;

 private:
  Colour fill;
  std::optional<Colour> outline;
};

class CircleOutline : public CircleBase {
 public:
  CircleOutline(Point center, double radius, Colour outline)
      : CircleBase(center, radius), outline_(std::move(outline)) {}

  virtual void draw() override;

 private:
  Colour outline_;
};

template <unsigned Vertices>
class Polygon : public Shape {
 public:
  Polygon(std::array<Point, Vertices> vertices, Colour outline)
      : vertices_(std::move(vertices)), outline_(outline) {}
  virtual void draw() override;

 protected:
  std::array<Point, Vertices> vertices_;
  Colour outline_;
};

template <unsigned Vertices>
class ConvexPolygon : public Polygon {
 public:
  ConvexPolygon(std::array<Point, Vertices> vertices, Colour outline)
      : Polygon(std::move(vertices), outline) {}
  virtual void draw() override;

 protected:
};

template <unsigned Vertices>
class RegularPolygon : public Polygon {
 public:
  RegularPolygon(Point center, uint16_t sideLength, double rotation = 0.0, Colour outline)
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
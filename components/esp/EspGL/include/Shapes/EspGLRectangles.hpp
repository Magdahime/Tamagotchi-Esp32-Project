#pragma once
#include <stdint.h>

#include <optional>
#include <stdexcept>

#include "EspGL.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"
#include "EspGlScreen.hpp"
#include "Shapes/EspGLRectangles.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class RectangleBase : public Shape<ColourRepresentation> {
 public:
  RectangleBase(Point leftUpperPoint, int32_t dimensionX, int32_t dimensionY)
      : leftUpperPoint_(std::move(leftUpperPoint)),
        rightLowerPoint_(
            {leftUpperPoint.x_ + dimensionX, leftUpperPoint.y_ + dimensionY}),
        dimensionX_(std::move(dimensionX)),
        dimensionY_(std::move(dimensionY)) {
    if (dimensionX <= 0 || dimensionY <= 0) {
      throw std::invalid_argument(
          "Dimensions of Rectangle cannot be negative!");
    }
  }
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

 protected:
  Point leftUpperPoint_;
  Point rightLowerPoint_;
  int16_t dimensionX_;
  int16_t dimensionY_;
};

template <typename ColourRepresentation>
class Rectangle : public RectangleBase<ColourRepresentation> {
 public:
  Rectangle(Point leftUpperPoint, int32_t dimensionX, int32_t dimensionY,
            Colour<ColourRepresentation> fill,
            std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : RectangleBase<ColourRepresentation>(leftUpperPoint, dimensionX,
                                            dimensionY),
        fill_(fill),
        outline_(outline) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

template <typename ColourRepresentation>
class RectangleOutline : public RectangleBase<ColourRepresentation> {
 public:
  RectangleOutline(Point leftUpperPoint, int32_t dimensionX, int32_t dimensionY,
                   Colour<ColourRepresentation> outline)
      : RectangleBase<ColourRepresentation>(leftUpperPoint, dimensionX,
                                            dimensionY),
        outline_(outline) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation>
class Square : public Rectangle<ColourRepresentation> {
 public:
  Square(Point leftUpperPoint, int32_t dimension,
         Colour<ColourRepresentation> fill,
         std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : Rectangle<ColourRepresentation>(leftUpperPoint, dimension, dimension,
                                        fill, outline) {}
  Square(Point center, int32_t sideLength, Colour<ColourRepresentation> fill,
         double angle,
         std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : Rectangle<ColourRepresentation>(
            Point(center.x_ - cos(angle) * (sideLength / 2.0),
                  center.y_ - sin(angle) * (sideLength / 2.0)),
            sideLength, sideLength, fill, outline) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
};

template <typename ColourRepresentation>
class SquareOutline : public RectangleOutline<ColourRepresentation> {
 public:
  SquareOutline(Point leftUpperPoint, int32_t dimension,
                Colour<ColourRepresentation> outline)
      : RectangleOutline<ColourRepresentation>(leftUpperPoint, dimension,
                                               dimension, outline) {}
  SquareOutline(Point center, int32_t sideLength,
                Colour<ColourRepresentation> outline, double angle)
      : RectangleOutline<ColourRepresentation>(
            Point(center.x_ - cos(angle) * (sideLength / 2.0),
                  center.y_ - sin(angle) * (sideLength / 2.0)),
            sideLength, sideLength, outline) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
};

/////////////////////////////
///       RECTANGLE       ///
/////////////////////////////

template <typename ColourRepresentation>
void Rectangle<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  if (this->rightLowerPoint_.x_ >= target.width() ||
      this->rightLowerPoint_.y_ >= target.height()) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixel()! Rectangle "
             "vertices are out of bound!");
    return;
  }
  uint16_t beginX = this->leftUpperPoint_.x_;
  uint16_t endX = this->rightLowerPoint_.x_;
  uint16_t beginY = this->leftUpperPoint_.y_;
  uint16_t endY = this->rightLowerPoint_.y_;
  if (endX >= target.width()) endX = target.width() - 1;
  if (endY >= target.height()) endY = target.width() - 1;
  ESP_LOGD(TAG_,
           "Drawing filled rectangle of vertices: (%d,%d) (%d,%d) (%d,%d) "
           "(%d,%d) in "
           "colour: 0x%X",
           beginX, beginY, endX, beginY, endX, endY, beginX, endY,
           fill_.value());
  target.screenDriver()->writePixelArea(beginX, endX, beginY, endY,
                                        fill_.value());
  if (outline_) {
    RectangleOutline<ColourRepresentation>(this->leftUpperPoint_,
                                           this->dimensionX_, this->dimensionY_,
                                           this->outline_.value())
        .draw(target);
  }
}

template <typename ColourRepresentation>
void RectangleOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  ESP_LOGD(
      TAG_,
      "Drawing filled rectangle of vertices: (%d,%d) (%d,%d) (%d,%d) "
      "(%d,%d) in "
      "colour: 0x%X",
      this->leftUpperPoint_.x_, this->leftUpperPoint_.y_,
      this->leftUpperPoint_.x_ + this->dimensionX_, this->leftUpperPoint_.y_,
      this->leftUpperPoint_.x_ + this->dimensionX_,
      this->leftUpperPoint_.y_ + this->dimensionY_, this->leftUpperPoint_.x_,
      this->leftUpperPoint_.y_ + this->dimensionY_, outline_.value());
  Line<ColourRepresentation>{
      Point(this->leftUpperPoint_.x_, this->leftUpperPoint_.y_),
      Point(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_),
      outline_.value()}
      .draw(target);
  Line<ColourRepresentation>{
      Point(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_),
      Point(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      outline_.value()}
      .draw(target);
  Line<ColourRepresentation>{
      Point(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      Point(this->leftUpperPoint_.x_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      outline_.value()}
      .draw(target);
  Line<ColourRepresentation>{
      Point(this->leftUpperPoint_.x_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      Point(this->leftUpperPoint_.x_, this->leftUpperPoint_.y_),
      outline_.value()}
      .draw(target);
}

/////////////////////////////
///        SQUARE         ///
/////////////////////////////

template <typename ColourRepresentation>
void Square<ColourRepresentation>::draw(Screen<ColourRepresentation>& target) {
  Rectangle<ColourRepresentation>::draw(target);
}

template <typename ColourRepresentation>
void SquareOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  RectangleOutline<ColourRepresentation>::draw(target);
}

}  // namespace EspGL
}  // namespace tamagotchi
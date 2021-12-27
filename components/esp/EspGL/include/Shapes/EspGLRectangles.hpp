#pragma once
#include <stdint.h>

#include "EspGL.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"
#include "EspGlScreen.hpp"

namespace tamagotchi {
namespace EspGL {

// Poprawić tutaj double może być większy i po std::round do int16_t może się
// przekręcić

template <typename ColourRepresentation>
class RectangleBase : public Shape<ColourRepresentation> {
 public:
  RectangleBase(Point leftUpperPoint, double dimensionX, double dimensionY)
      : leftUpperPoint_(std::move(leftUpperPoint)),
  rightLowerPoint_(
      {leftUpperPoint.x_ + dimensionX, leftUpperPoint.y_ + dimensionY}),
  dimensionX_(std::round(dimensionX)),
  dimensionY_(std::round(dimensionY)) {}
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
  Rectangle(Point leftUpperPoint, double dimensionX, double dimensionY,
            Colour<ColourRepresentation> fill)
      : RectangleBase<ColourRepresentation>(leftUpperPoint, dimensionX,
                                            dimensionY),
        fill_(fill) {}
  Rectangle(Point leftUpperPoint, double dimensionX, double dimensionY,
            Colour<ColourRepresentation> fill,
            Colour<ColourRepresentation> outline)
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
  RectangleOutline(Point leftUpperPoint, double dimensionX, double dimensionY,
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
  Square(Point leftUpperPoint, double dimensionX, double dimensionY,
         Colour<ColourRepresentation> fill)
      : Rectangle<ColourRepresentation>(leftUpperPoint, dimensionX, dimensionY,
                                        fill) {}
  Square(Point leftUpperPoint, double dimensionX, double dimensionY,
         Colour<ColourRepresentation> fill,
         Colour<ColourRepresentation> outline)
      : Rectangle<ColourRepresentation>(leftUpperPoint, dimensionX, dimensionY,
                                        fill, outline) {}
  Square(Point center, double sideLength, Colour<ColourRepresentation> fill,
         double angle = 0.0);
  Square(Point center, double sideLength, Colour<ColourRepresentation> fill,
         Colour<ColourRepresentation> outline, double angle = 0.0);
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
};

template <typename ColourRepresentation>
class SquareOutline : public RectangleOutline<ColourRepresentation> {
 public:
  SquareOutline(Point leftUpperPoint, double dimensionX, double dimensionY,
                Colour<ColourRepresentation> outline)
      : RectangleOutline<ColourRepresentation>(leftUpperPoint, dimensionX,
                                               dimensionY, outline) {}
  SquareOutline(Point center, double sideLength,
                Colour<ColourRepresentation> outline, double angle = 0.0);
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
};

/////////////////////////////
///       RECTANGLE       ///
/////////////////////////////

template <typename ColourRepresentation>
void Rectangle<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  EspGL::Point min_point = {
      std::min(this->leftUpperPoint_.x_, this->rightLowerPoint_.x_),
      std::min(this->leftUpperPoint_.y_, this->rightLowerPoint_.y_)};
  EspGL::Point max_point = {
      std::max(this->leftUpperPoint_.x_, this->rightLowerPoint_.x_),
      std::max(this->leftUpperPoint_.y_, this->rightLowerPoint_.y_)};
  if (min_point.x_ >= target.width() || min_point.y_ >= target.height()) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixel()! Rectangle "
             "vertices are out of bound!");
    return;
  }
  uint16_t beginX = min_point.x_;
  uint16_t endX = max_point.x_;
  uint16_t beginY = min_point.y_;
  uint16_t endY = max_point.y_;
  uint16_t size = endX - beginX + 1;
  if (max_point.x_ >= target.width()) endX = target.width() - 1;
  if (max_point.y_ >= target.height()) endY = target.height() - 1;
  ESP_LOGD(TAG_,
           "Drawing filled rectangle of vertices: (%d,%d) (%d,%d) (%d,%d) "
           "(%d,%d) in "
           "colour: 0x%X",
           beginX, beginY, endX, beginY, endX, endY, beginX, endY,
           fill_.value());
  ESP_LOGD(TAG_, "Length of the side: %d ", size);
  target.screenDriver()->writePixelArea(beginX, endX, beginY, endY, fill_);
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
  Line{{this->leftUpperPoint_.x_, this->leftUpperPoint_.y_},
       {this->leftUpperPoint_.x_ + this->dimensionX_, this->leftUpperPoint_.y_},
       outline_.value()}
      .draw(target);
  Line{{this->leftUpperPoint_.x_ + this->dimensionX_, this->leftUpperPoint_.y_},
       {this->leftUpperPoint_.x_ + this->dimensionX_,
        this->leftUpperPoint_.y_ + this->dimensionY_},
       outline_.value()}
      .draw(target);
  Line{{this->leftUpperPoint_.x_ + this->dimensionX_,
        this->leftUpperPoint_.y_ + this->dimensionY_},
       {this->leftUpperPoint_.x_, this->leftUpperPoint_.y_ + this->dimensionY_},
       outline_.value()}
      .draw(target);
  Line{{this->leftUpperPoint_.x_, this->leftUpperPoint_.y_ + this->dimensionY_},
       {this->leftUpperPoint_.x_, this->leftUpperPoint_.y_},
       outline_.value()}
      .draw(target);
}

/////////////////////////////
///        SQUARE         ///
/////////////////////////////

template <typename ColourRepresentation>
Square<ColourRepresentation>::Square(Point center, double sideLength,
                                     Colour<ColourRepresentation> fill,
                                     double angle) {}

template <typename ColourRepresentation>
Square<ColourRepresentation>::Square(Point center, double sideLength,
                                     Colour<ColourRepresentation> fill,
                                     Colour<ColourRepresentation> outline,
                                     double angle) {}

template <typename ColourRepresentation>
void Square<ColourRepresentation>::draw(Screen<ColourRepresentation>& target) {
  Rectangle<ColourRepresentation>::draw(target);
}

template <typename ColourRepresentation>
SquareOutline<ColourRepresentation>::SquareOutline(
    Point center, double sideLength, Colour<ColourRepresentation> outline,
    double angle) {}

template <typename ColourRepresentation>
void SquareOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  RectangleOutline<ColourRepresentation>::draw(target);
}

}  // namespace EspGL
}  // namespace tamagotchi
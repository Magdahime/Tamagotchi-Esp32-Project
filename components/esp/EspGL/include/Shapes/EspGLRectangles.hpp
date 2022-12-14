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
  RectangleBase(Vect2 leftUpperPoint, int16_t dimensionX, int16_t dimensionY)
      : leftUpperPoint_(std::move(leftUpperPoint)),
        rightLowerPoint_({leftUpperPoint.x_ + dimensionX - 1,
                          leftUpperPoint.y_ + dimensionY - 1}),
        dimensionX_(std::move(dimensionX)),
        dimensionY_(std::move(dimensionY)) {
    if (dimensionX <= 0 || dimensionY <= 0) {
      ESP_LOGE("RectangleBase", "Dimensions of Rectangle cannot be negative!");
    }
  }
  virtual ~RectangleBase() = default;
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

  inline const Vect2& leftUpperPoint() const { return leftUpperPoint_; }
  inline const Vect2& rightLowerPoint() const { return rightLowerPoint_; }
  inline int16_t dimensionX() { return dimensionX_; }
  inline int16_t dimensionY() { return dimensionY_; }

  inline void setLeftUpperPoint(Vect2 leftUpperPoint) {
    leftUpperPoint_ = leftUpperPoint;
  }
  inline void setRightLowerPoint(Vect2 rightLowerPoint) {
    rightLowerPoint_ = rightLowerPoint;
  }
  inline void setDimensionX(int16_t dimensionX) { dimensionX_ = dimensionX; }
  inline void setDimensionY(int16_t dimensionY) { dimensionY_ = dimensionY; }

  virtual inline Hitbox hitbox() override {
    return std::make_pair(leftUpperPoint_, rightLowerPoint_);
  }

 protected:
  Vect2 leftUpperPoint_;
  Vect2 rightLowerPoint_;
  int16_t dimensionX_;
  int16_t dimensionY_;
};

template <typename ColourRepresentation>
class Rectangle : public RectangleBase<ColourRepresentation> {
 public:
  Rectangle(Vect2 leftUpperPoint, int16_t dimensionX, int16_t dimensionY,
            Colour<ColourRepresentation> fill,
            std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : RectangleBase<ColourRepresentation>(leftUpperPoint, dimensionX,
                                            dimensionY),
        fill_(fill),
        outline_(outline) {}
  virtual ~Rectangle() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline const Colour<ColourRepresentation>& fill() const { return fill_; }
  inline const Colour<ColourRepresentation>& outline() const {
    return outline_;
  }

  inline void setFill(Colour<ColourRepresentation> newFill) { fill_ = newFill; }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 protected:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

template <typename ColourRepresentation>
class RectangleOutline : public RectangleBase<ColourRepresentation> {
 public:
  RectangleOutline(Vect2 leftUpperPoint, int16_t dimensionX, int16_t dimensionY,
                   Colour<ColourRepresentation> outline)
      : RectangleBase<ColourRepresentation>(leftUpperPoint, dimensionX,
                                            dimensionY),
        outline_(outline) {}
  virtual ~RectangleOutline() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline const Colour<ColourRepresentation>& outline() const {
    return outline_;
  }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 protected:
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation>
class Square : public Rectangle<ColourRepresentation> {
 public:
  Square(Vect2 leftUpperPoint, int16_t dimension,
         Colour<ColourRepresentation> fill,
         std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : Rectangle<ColourRepresentation>(leftUpperPoint, dimension, dimension,
                                        fill, outline) {}
  Square(Vect2 center, int16_t sideLength, Colour<ColourRepresentation> fill,
         double angle,
         std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : Rectangle<ColourRepresentation>(
            Vect2(center.x_ - cos(angle) * (sideLength / 2.0),
                  center.y_ - sin(angle) * (sideLength / 2.0)),
            sideLength, sideLength, fill, outline) {}
  virtual ~Square() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
};

template <typename ColourRepresentation>
class SquareOutline : public RectangleOutline<ColourRepresentation> {
 public:
  SquareOutline(Vect2 leftUpperPoint, int16_t dimension,
                Colour<ColourRepresentation> outline)
      : RectangleOutline<ColourRepresentation>(leftUpperPoint, dimension,
                                               dimension, outline) {}
  SquareOutline(Vect2 center, int16_t sideLength,
                Colour<ColourRepresentation> outline, double angle)
      : RectangleOutline<ColourRepresentation>(
            Vect2(center.x_ - cos(angle) * (sideLength / 2.0),
                  center.y_ - sin(angle) * (sideLength / 2.0)),
            sideLength, sideLength, outline) {}
  virtual ~SquareOutline() = default;
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
      Vect2(this->leftUpperPoint_.x_, this->leftUpperPoint_.y_),
      Vect2(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_),
      outline_.value()}
      .draw(target);
  Line<ColourRepresentation>{
      Vect2(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_),
      Vect2(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      outline_.value()}
      .draw(target);
  Line<ColourRepresentation>{
      Vect2(this->leftUpperPoint_.x_ + this->dimensionX_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      Vect2(this->leftUpperPoint_.x_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      outline_.value()}
      .draw(target);
  Line<ColourRepresentation>{
      Vect2(this->leftUpperPoint_.x_,
            this->leftUpperPoint_.y_ + this->dimensionY_),
      Vect2(this->leftUpperPoint_.x_, this->leftUpperPoint_.y_),
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
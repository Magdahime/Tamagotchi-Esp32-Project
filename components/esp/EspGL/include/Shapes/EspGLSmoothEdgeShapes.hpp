#pragma once
#include "EspGL.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"
#include "EspGLScreen.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class CircleBase : public Shape<ColourRepresentation> {
 public:
  CircleBase(Point center, double radius)
      : center_(std::move(center)), radius_(std::move(radius)) {}
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

 protected:
  Point center_;
  double radius_;
};

template <typename ColourRepresentation>
class Circle : public CircleBase<ColourRepresentation> {
 public:
  Circle(Point center, double radius, Colour<ColourRepresentation> fill)
      : CircleBase<ColourRepresentation>(center, radius),
        fill_(std::move(fill)) {}
  Circle(Point center, double radius, Colour<ColourRepresentation> fill,
         Colour<ColourRepresentation> outline)
      : CircleBase<ColourRepresentation>(center, radius),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

template <typename ColourRepresentation>
class CircleOutline : public CircleBase<ColourRepresentation> {
 public:
  CircleOutline(Point center, double radius,
                Colour<ColourRepresentation> outline)
      : CircleBase<ColourRepresentation>(center, radius),
        outline_(std::move(outline)) {}

  virtual void draw(Screen<ColourRepresentation>& target) override;

 private:
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation>
class EllipseBase : public Shape<ColourRepresentation> {
 public:
  EllipseBase(Point center, double xRadius, double yRadius)
      : center_(std::move(center)),
        xRadius_(std::move(xRadius)),
        yRadius_(std::move(yRadius)) {}
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

 protected:
  Point center_;
  double xRadius_;
  double yRadius_;
};

template <typename ColourRepresentation>
class Ellipse : public EllipseBase<ColourRepresentation> {
 public:
  Ellipse(Point center, double xRadius, double yRadius,
          Colour<ColourRepresentation> fill)
      : EllipseBase<ColourRepresentation>(center, xRadius, yRadius),
        fill_(std::move(fill)) {}
  Ellipse(Point center, double xRadius, double yRadius,
          Colour<ColourRepresentation> fill,
          Colour<ColourRepresentation> outline)
      : EllipseBase<ColourRepresentation>(center, xRadius, yRadius),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

template <typename ColourRepresentation>
class EllipseOutline : public EllipseBase<ColourRepresentation> {
 public:
  EllipseOutline(Point center, double xRadius, double yRadius,
                 Colour<ColourRepresentation> outline)
      : EllipseBase<ColourRepresentation>(center, xRadius, yRadius),
        outline_(std::move(outline)) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

 protected:
  Colour<ColourRepresentation> outline_;
};

/////////////////////////////
///        CIRCLE         ///
/////////////////////////////
template <typename ColourRepresentation>
void Circle<ColourRepresentation>::draw(Screen<ColourRepresentation>& target) {
  ESP_LOGD(TAG_,
           "Drawing filled circle of this->center_ (%d,%d) and radius %f in "
           "colour: 0x%X",
           this->center_.x_, this->center_.y_, this->radius_, fill_.value());
  Colour16 outline = fill_;
  if (outline_) {
    outline = outline_->value();
  }
  int16_t x = this->radius_;
  int16_t y = 0;
  int16_t err = 0;
  while (x >= y) {
    Line{{this->center_.x_ + x, this->center_.y_ + y},
         {this->center_.x_ - x, this->center_.y_ + y},
         fill_.value()}
        .draw(target);
    Line{{this->center_.x_ + y, this->center_.y_ + x},
         {this->center_.x_ - y, this->center_.y_ + x},
         fill_.value()}
        .draw(target);
    Line{{this->center_.x_ - x, this->center_.y_ - y},
         {this->center_.x_ + x, this->center_.y_ - y},
         fill_.value()}
        .draw(target);
    Line{{this->center_.x_ - y, this->center_.y_ - x},
         {this->center_.x_ + y, this->center_.y_ - x},
         fill_.value()}
        .draw(target);

    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }

    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

template <typename ColourRepresentation>
void CircleOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  ESP_LOGD(TAG_,
           "Drawing circle of center (%d,%d) and radius %f in "
           "colour: 0x%X",
           this->center_.x_, this->center_.y_, this->radius_, outline_.value());
  int16_t x = this->radius_;
  int16_t y = 0;
  int16_t err = 0;
  while (x >= y) {
    target.screenDriver()->drawPixel(
        {this->center_.x_ + x, this->center_.y_ + y}, outline_);
    target.screenDriver()->drawPixel(
        {this->center_.x_ + y, this->center_.y_ + x}, outline_);
    target.screenDriver()->drawPixel(
        {this->center_.x_ - y, this->center_.y_ + x}, outline_);
    target.screenDriver()->drawPixel(
        {this->center_.x_ - x, this->center_.y_ + y}, outline_);
    target.screenDriver()->drawPixel(
        {this->center_.x_ - x, this->center_.y_ - y}, outline_);
    target.screenDriver()->drawPixel(
        {this->center_.x_ - y, this->center_.y_ - x}, outline_);
    target.screenDriver()->drawPixel(
        {this->center_.x_ + y, this->center_.y_ - x}, outline_);
    target.screenDriver()->drawPixel(
        {this->center_.x_ + x, this->center_.y_ - y}, outline_);

    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }

    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

/////////////////////////////
///       ELLIPSE         ///
/////////////////////////////

template <typename ColourRepresentation>
void Ellipse<ColourRepresentation>::draw(Screen<ColourRepresentation>& target) {
  ESP_LOGD(TAG_,
           "Drawing filled ellipse of center (%d, %d) of radiuses (%f, %f) in "
           "colour: 0x%X",
           this->center_.x_, this->center_.y_, this->xRadius_, this->yRadius_,
           fill_.value());
  float dx, dy, d1, d2, x, y;
  x = 0;
  y = this->yRadius_;
  d1 = (this->yRadius_ * this->yRadius_) -
       (this->xRadius_ * this->xRadius_ * this->yRadius_) +
       (0.25 * this->xRadius_ * this->xRadius_);
  dx = 2 * this->yRadius_ * this->yRadius_ * x;
  dy = 2 * this->xRadius_ * this->xRadius_ * y;

  // For region 1
  while (dx < dy) {
    Line{{this->center_.x_ + x, this->center_.y_ + y},
         {this->center_.x_ - x, this->center_.y_ + y},
         fill_}
        .draw(target);
    Line{{x + this->center_.x_, -y + this->center_.y_},
         {-x + this->center_.x_, -y + this->center_.y_},
         fill_}
        .draw(target);

    if (d1 < 0) {
      x++;
      dx = dx + (2 * this->yRadius_ * this->yRadius_);
      d1 = d1 + dx + (this->yRadius_ * this->yRadius_);
    } else {
      x++;
      y--;
      dx = dx + (2 * this->yRadius_ * this->yRadius_);
      dy = dy - (2 * this->xRadius_ * this->xRadius_);
      d1 = d1 + dx - dy + (this->yRadius_ * this->yRadius_);
    }
  }

  d2 = ((this->yRadius_ * this->yRadius_) * ((x + 0.5) * (x + 0.5))) +
       ((this->xRadius_ * this->xRadius_) * ((y - 1) * (y - 1))) -
       (this->xRadius_ * this->xRadius_ * this->yRadius_ * this->yRadius_);

  // Plotting points of region 2
  while (y >= 0) {
    Line{{this->center_.x_ + x, this->center_.y_ + y},
         {this->center_.x_ - x, this->center_.y_ + y},
         fill_}
        .draw(target);
    Line{{x + this->center_.x_, -y + this->center_.y_},
         {-x + this->center_.x_, -y + this->center_.y_},
         fill_}
        .draw(target);

    if (d2 > 0) {
      y--;
      dy = dy - (2 * this->xRadius_ * this->xRadius_);
      d2 = d2 + (this->xRadius_ * this->xRadius_) - dy;
    } else {
      y--;
      x++;
      dx = dx + (2 * this->yRadius_ * this->yRadius_);
      dy = dy - (2 * this->xRadius_ * this->xRadius_);
      d2 = d2 + dx - dy + (this->xRadius_ * this->xRadius_);
    }
  }
}

template <typename ColourRepresentation>
void EllipseOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  ESP_LOGD(
      TAG_,
      "Drawing ellipse of center (%d, %d) of radiuses (%f, %f) in colour: 0x%X",
      this->center_.x_, this->center_.y_, this->xRadius_, this->yRadius_,
      outline_.value());
  float dx, dy, d1, d2, x, y;
  x = 0;
  y = this->yRadius_;
  d1 = (this->yRadius_ * this->yRadius_) -
       (this->xRadius_ * this->xRadius_ * this->yRadius_) +
       (0.25 * this->xRadius_ * this->xRadius_);
  dx = 2 * this->yRadius_ * this->yRadius_ * x;
  dy = 2 * this->xRadius_ * this->xRadius_ * y;

  // For region 1
  while (dx < dy) {
    target.screenDriver()->drawPixel(
        {x + this->center_.x_, y + this->center_.y_}, outline_);
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, y + this->center_.y_}, outline_);
    target.screenDriver()->drawPixel(
        {x + this->center_.x_, -y + this->center_.y_}, outline_);
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, -y + this->center_.y_}, outline_);

    if (d1 < 0) {
      x++;
      dx = dx + (2 * this->yRadius_ * this->yRadius_);
      d1 = d1 + dx + (this->yRadius_ * this->yRadius_);
    } else {
      x++;
      y--;
      dx = dx + (2 * this->yRadius_ * this->yRadius_);
      dy = dy - (2 * this->xRadius_ * this->xRadius_);
      d1 = d1 + dx - dy + (this->yRadius_ * this->yRadius_);
    }
  }

  d2 = ((this->yRadius_ * this->yRadius_) * ((x + 0.5) * (x + 0.5))) +
       ((this->xRadius_ * this->xRadius_) * ((y - 1) * (y - 1))) -
       (this->xRadius_ * this->xRadius_ * this->yRadius_ * this->yRadius_);

  // Plotting points of region 2
  while (y >= 0) {
    target.screenDriver()->drawPixel(
        {x + this->center_.x_, y + this->center_.y_}, outline_);
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, y + this->center_.y_}, outline_);
    target.screenDriver()->drawPixel(
        {x + this->center_.x_, -y + this->center_.y_}, outline_);
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, -y + this->center_.y_}, outline_);

    if (d2 > 0) {
      y--;
      dy = dy - (2 * this->xRadius_ * this->xRadius_);
      d2 = d2 + (this->xRadius_ * this->xRadius_) - dy;
    } else {
      y--;
      x++;
      dx = dx + (2 * this->yRadius_ * this->yRadius_);
      dy = dy - (2 * this->xRadius_ * this->xRadius_);
      d2 = d2 + dx - dy + (this->xRadius_ * this->xRadius_);
    }
  }
}

}  // namespace EspGL
}  // namespace tamagotchi
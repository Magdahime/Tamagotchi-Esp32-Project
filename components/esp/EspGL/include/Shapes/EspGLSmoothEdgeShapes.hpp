#pragma once
#include <optional>

#include "EspGL.hpp"
#include "EspGLScreen.hpp"
#include "EspGLSimpleShapes.hpp"
#include "EspGLUtils.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class CircleBase : public Shape<ColourRepresentation> {
 public:
  CircleBase(Vect2 center, double radius)
      : center_(std::move(center)), radius_(std::move(radius)) {}
  virtual ~CircleBase() = default;
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

  inline const Vect2& center() const { return center_; }
  inline double radius() { return radius_; }

  inline void setCenter(Vect2 newCenter) { center_ = newCenter; }
  inline void setRadius(double newRadius) { radius_ = newRadius; }

  virtual inline std::pair<Vect2, Vect2> hitbox() override {
    return std::make_pair(Vect2{this->center_.x_ - this->radius_,
                                this->center_.y_ - this->radius_},
                          Vect2{this->center_.x_ + this->radius_,
                                this->center_.y_ + this->radius_});
  }

 protected:
  Vect2 center_;
  double radius_;
};

template <typename ColourRepresentation>
class Circle : public CircleBase<ColourRepresentation> {
 public:
  Circle(Vect2 center, double radius, Colour<ColourRepresentation> fill,
         std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : CircleBase<ColourRepresentation>(center, radius),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}
  virtual ~Circle() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline const Colour<ColourRepresentation>& fill() const { return fill_; }
  inline const Colour<ColourRepresentation>& outline() const {
    return outline_.value();
  }

  inline void setFill(Colour<ColourRepresentation> newFill) { fill_ = newFill; }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 private:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

template <typename ColourRepresentation>
class CircleOutline : public CircleBase<ColourRepresentation> {
 public:
  CircleOutline(Vect2 center, double radius,
                Colour<ColourRepresentation> outline)
      : CircleBase<ColourRepresentation>(center, radius),
        outline_(std::move(outline)) {}
  virtual ~CircleOutline() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline const Colour<ColourRepresentation>& outline() const {
    return outline_.value();
  }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 private:
  Colour<ColourRepresentation> outline_;
};

template <typename ColourRepresentation>
class EllipseBase : public Shape<ColourRepresentation> {
 public:
  EllipseBase(Vect2 center, double xRadius, double yRadius)
      : center_(std::move(center)),
        xRadius_(std::move(xRadius)),
        yRadius_(std::move(yRadius)) {}
  virtual ~EllipseBase() = default;
  virtual void draw(Screen<ColourRepresentation>& target) = 0;

  inline const Vect2& center() const { return center_; }
  inline double xRadius() { return xRadius_; }
  inline double yRadius() { return yRadius_; }

  inline void setCenter(Vect2 newCenter) { center_ = newCenter; }
  inline void setXRadius(double newRadius) { xRadius_ = newRadius; }
  inline void setYRadius(double newRadius) { yRadius_ = newRadius; }

  virtual inline std::pair<Vect2, Vect2> hitbox() override {
    return std::make_pair(Vect2(this->center_.x_ - this->xRadius_,
                                this->center_.y_ - this->yRadius_),
                          Vect2(this->center_.x_ + this->xRadius_,
                                this->center_.y_ + this->yRadius_));
  }

 protected:
  Vect2 center_;
  double xRadius_;
  double yRadius_;
};

template <typename ColourRepresentation>
class Ellipse : public EllipseBase<ColourRepresentation> {
 public:
  Ellipse(Vect2 center, double xRadius, double yRadius,
          Colour<ColourRepresentation> fill,
          std::optional<Colour<ColourRepresentation>> outline = std::nullopt)
      : EllipseBase<ColourRepresentation>(center, xRadius, yRadius),
        fill_(std::move(fill)),
        outline_(std::move(outline)) {}
  virtual ~Ellipse() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline Colour<ColourRepresentation>& fill() { return fill_; }
  inline Colour<ColourRepresentation>& outline() { return outline_.value(); }

  inline void setFill(Colour<ColourRepresentation> newFill) { fill_ = newFill; }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

 protected:
  Colour<ColourRepresentation> fill_;
  std::optional<Colour<ColourRepresentation>> outline_;
};

template <typename ColourRepresentation>
class EllipseOutline : public EllipseBase<ColourRepresentation> {
 public:
  EllipseOutline(Vect2 center, double xRadius, double yRadius,
                 Colour<ColourRepresentation> outline)
      : EllipseBase<ColourRepresentation>(center, xRadius, yRadius),
        outline_(std::move(outline)) {}

  virtual ~EllipseOutline() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline Colour<ColourRepresentation>& outline() { return outline_.value(); }
  inline void setOutline(Colour<ColourRepresentation> newOutline) {
    outline_ = newOutline;
  }

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
    Line<ColourRepresentation>{{this->center_.x_ + x, this->center_.y_ + y},
                               {this->center_.x_ - x, this->center_.y_ + y},
                               fill_.value()}
        .draw(target);
    Line<ColourRepresentation>{{this->center_.x_ + y, this->center_.y_ + x},
                               {this->center_.x_ - y, this->center_.y_ + x},
                               fill_.value()}
        .draw(target);
    Line<ColourRepresentation>{{this->center_.x_ - x, this->center_.y_ - y},
                               {this->center_.x_ + x, this->center_.y_ - y},
                               fill_.value()}
        .draw(target);
    Line<ColourRepresentation>{{this->center_.x_ - y, this->center_.y_ - x},
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
  if (outline_) {
    CircleOutline<ColourRepresentation>{this->center_, this->radius_,
                                        this->outline_.value()}
        .draw(target);
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
        {this->center_.x_ + x, this->center_.y_ + y}, outline_.value());
    target.screenDriver()->drawPixel(
        {this->center_.x_ + y, this->center_.y_ + x}, outline_.value());
    target.screenDriver()->drawPixel(
        {this->center_.x_ - y, this->center_.y_ + x}, outline_.value());
    target.screenDriver()->drawPixel(
        {this->center_.x_ - x, this->center_.y_ + y}, outline_.value());
    target.screenDriver()->drawPixel(
        {this->center_.x_ - x, this->center_.y_ - y}, outline_.value());
    target.screenDriver()->drawPixel(
        {this->center_.x_ - y, this->center_.y_ - x}, outline_.value());
    target.screenDriver()->drawPixel(
        {this->center_.x_ + y, this->center_.y_ - x}, outline_.value());
    target.screenDriver()->drawPixel(
        {this->center_.x_ + x, this->center_.y_ - y}, outline_.value());

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
    Line<ColourRepresentation>{{this->center_.x_ + x, this->center_.y_ + y},
                               {this->center_.x_ - x, this->center_.y_ + y},
                               fill_}
        .draw(target);
    Line<ColourRepresentation>{{x + this->center_.x_, -y + this->center_.y_},
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
    Line<ColourRepresentation>{{this->center_.x_ + x, this->center_.y_ + y},
                               {this->center_.x_ - x, this->center_.y_ + y},
                               fill_}
        .draw(target);
    Line<ColourRepresentation>{{x + this->center_.x_, -y + this->center_.y_},
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
  if (outline_) {
    EllipseOutline<ColourRepresentation>{this->center_, this->xRadius_,
                                         this->yRadius_, this->outline_.value()}
        .draw(target);
  }
}

template <typename ColourRepresentation>
void EllipseOutline<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  ESP_LOGD(TAG_,
           "Drawing ellipse of center (%d, %d) of radiuses (%f, %f) in "
           "colour: 0x%X",
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
        {x + this->center_.x_, y + this->center_.y_}, outline_.value());
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, y + this->center_.y_}, outline_.value());
    target.screenDriver()->drawPixel(
        {x + this->center_.x_, -y + this->center_.y_}, outline_.value());
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, -y + this->center_.y_}, outline_.value());

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
        {x + this->center_.x_, y + this->center_.y_}, outline_.value());
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, y + this->center_.y_}, outline_.value());
    target.screenDriver()->drawPixel(
        {x + this->center_.x_, -y + this->center_.y_}, outline_.value());
    target.screenDriver()->drawPixel(
        {-x + this->center_.x_, -y + this->center_.y_}, outline_.value());

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
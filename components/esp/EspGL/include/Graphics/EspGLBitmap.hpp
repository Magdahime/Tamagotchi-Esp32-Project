#pragma once
#include <stdint.h>

#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "Drawable.hpp"
#include "EspGL.hpp"
#include "EspGLUtils.hpp"
#include "EspGlScreen.hpp"

namespace tamagotchi {
namespace EspGL {
class Bitmap {
 public:
  Bitmap(size_t sizeX, size_t sizeY, std::vector<bool> bitmap)
      : sizeX_(std::move(sizeX)),
        sizeY_(std::move(sizeY)),
        bitmap_(std::move(bitmap)) {
    if (bitmap_.size() != sizeX_ * sizeY_) {
      throw std::runtime_error(
          "Invalid std::vector<bool> passed to Bitmap constructor. Size is not "
          "equal to provided values.");
    }
  }
  Bitmap() = default;
  virtual ~Bitmap() = default;
  template <typename ColourRepresentation>
  void draw(
      Screen<ColourRepresentation>& target, const Point start,
      Colour<ColourRepresentation> colour,
      std::optional<Colour<ColourRepresentation>> background = std::nullopt);

  template <typename ColourRepresentation>
  void drawScaled(
      Screen<ColourRepresentation>& target, Point start,
      Colour<ColourRepresentation> colour, uint32_t scale,
      std::optional<Colour<ColourRepresentation>> background = std::nullopt);

  void overlay(Bitmap& bitmap, Point& start);

  inline size_t& sizeX() { return sizeX_; }
  inline size_t& sizeY() { return sizeY_; }
  inline std::vector<bool>& bitmap() { return bitmap_; };
  inline bool empty() const { return bitmap_.empty(); }
  inline bool at(int64_t index) const { return bitmap_[index]; }

 private:
  size_t sizeX_;
  size_t sizeY_;
  std::vector<bool> bitmap_;
};

template <typename ColourRepresentation>
void Bitmap::draw(Screen<ColourRepresentation>& target, const Point start,
                  Colour<ColourRepresentation> colour,
                  std::optional<Colour<ColourRepresentation>> background) {
  ESP_LOGD(TAG_, "Drawing bitmap of size (%zu, %zu)in colour: 0x%X", sizeX_,
           sizeY_, colour.value());
  uint16_t countX = 0, countY = 0;
  for (const auto& pixel : bitmap_) {
    if (pixel) {
      target.screenDriver()->drawPixel({start.x_ + countX, start.y_ + countY},
                                       colour.value());
    } else if (!pixel && background) {
      target.screenDriver()->drawPixel({start.x_ + countX, start.y_ + countY},
                                       background.value().value());
    }
    if (++countX == sizeX_) {
      countY++;
      countX = 0;
    }
  }
}

template <typename ColourRepresentation>
void Bitmap::drawScaled(
    Screen<ColourRepresentation>& target, Point start,
    Colour<ColourRepresentation> colour, uint32_t scale,
    std::optional<Colour<ColourRepresentation>> background) {
  ESP_LOGD(TAG_, "Drawing bitmap of size (%zu, %zu)in colour: 0x%X", sizeX_,
           sizeY_, colour.value());
  uint32_t offset = start.x_;
  size_t widthCounter = sizeX_ * scale;
  for (const auto& bit : bitmap_) {
    if (background && bit == false) {
      for (auto i = 0; i < scale; ++i) {
        target.screenDriver()->writePixelArea(start.x_, start.x_ + scale,
                                              start.y_ + i, start.y_ + i,
                                              background.value().value());
      }
    } else if (bit == true) {
      for (auto i = 0; i < scale; ++i) {
        target.screenDriver()->writePixelArea(start.x_, start.x_ + scale,
                                              start.y_ + i, start.y_ + i,
                                              colour.value());
      }
    }
    start.x_ += scale;
    widthCounter -= scale;
    if (widthCounter <= 0) {
      start.x_ = offset;
      start.y_ += scale;
      widthCounter = sizeX_ * scale;
    }
  }
}

template <typename ColourRepresentation>
class BitmapDrawable : public Drawable<ColourRepresentation> {
 public:
  BitmapDrawable(
      size_t sizeX, size_t sizeY, std::vector<bool> bitmap, Point start,
      int scale, Colour<ColourRepresentation> colour,
      std::optional<Colour<ColourRepresentation>> background = std::nullopt)
      : bitmap_(sizeX, sizeY, bitmap),
        start_(start),
        scale_(scale),
        colour_(colour),
        background_(background) {}
  BitmapDrawable(
      Bitmap& bitmap, Point start, int scale,
      Colour<ColourRepresentation> colour,
      std::optional<Colour<ColourRepresentation>> background = std::nullopt)
      : bitmap_(bitmap),
        start_(start),
        scale_(scale),
        colour_(colour),
        background_(background) {}

  virtual ~BitmapDrawable() = default;
  virtual void draw(Screen<ColourRepresentation>& target) override;

  Bitmap bitmap() { return bitmap_; }
  Point start() { return start_; }
  int scale() { return scale_; }
  Colour<ColourRepresentation> colour() { return colour_; }
  std::optional<Colour<ColourRepresentation>> background() {
    return background_;
  }

  void setColour(Colour<ColourRepresentation> newColour) {
    colour_ = newColour;
  }
  void setBackground(Colour<ColourRepresentation> newBackground) {
    background_ = newBackground;
  }

  virtual inline std::pair<Point, Point> hitbox() override {
    return std::make_pair(Point{start_.x_, start_.y_},
                          Point{start_.x_ + bitmap_.sizeX() * scale_,
                                start_.y_ + bitmap_.sizeY() * scale_});
  }

 private:
  Bitmap bitmap_;
  Point start_;
  int scale_;
  Colour<ColourRepresentation> colour_;
  std::optional<Colour<ColourRepresentation>> background_;
};

template <typename ColourRepresentation>
void BitmapDrawable<ColourRepresentation>::draw(
    Screen<ColourRepresentation>& target) {
  bitmap_.drawScaled(target, start_, colour_, scale_, background_);
}

}  // namespace EspGL
}  // namespace tamagotchi
#pragma once

#include <ctype.h>
#include <stdint.h>

#include <optional>
#include <string>

#include "../Font/EspGLFont.hpp"
#include "Drawable.hpp"
#include "EspGLScreen.hpp"
#include "EspGLUtils.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Text : Drawable<ColourRepresentation> {
 public:
  Text(std::string text, std::pair<Point, Point> textBox, Font& font,
       Colour<ColourRepresentation> colour, uint32_t characterScale = 3,
       std::optional<Colour<ColourRepresentation>> background = std::nullopt,
       uint16_t letterSpacing = 5, uint16_t lineSpacing = 2)
      : textBox_(textBox),
        letterSpacing_(letterSpacing),
        lineSpacing_(lineSpacing),
        characterScale_(characterScale),
        colour_(colour),
        background_(background),
        text_(std::move(text)),
        font_(font) {}
  Text(Font& font) : font_(font) {}
  virtual void draw(Screen<ColourRepresentation>& target) override;

  inline uint16_t letterSpacing() { return letterSpacing_; }
  inline uint16_t lineSpacing() { return lineSpacing_; }
  inline uint32_t characterScale() { return characterScale_; }
  inline const Colour<ColourRepresentation>& colour() const { return colour_; }
  inline const std::string& text() const { return text_; }
  inline const Font& font() const { return font_; }
  inline const Point& start() const { return textBox_.first; }
  inline const Point& end() const { return textBox_.second; }
  inline const std::pair<Point, Point>& textBox() const { return textBox_; }

  inline void setLetterSpacing(uint16_t letterSpacing) {
    letterSpacing_ = letterSpacing;
  }
  inline void setLineSpacing(uint16_t lineSpacing) {
    lineSpacing_ = lineSpacing;
  }
  inline void setCharacterScale(uint32_t characterScale) {
    characterScale_ = characterScale;
  }
  inline void setColour(Colour<ColourRepresentation> colour) {
    colour_ = colour;
  }
  inline void setText(std::string text) { text_ = text; }
  inline void setFont(Font font) { font_ = font; }
  inline void setStart(Point start) { textBox_.first = start; }
  inline void setEnd(Point end) { textBox_.second = end; }
  inline void setTextBox(std::pair<Point, Point> textBox) {
    textBox_ = textBox;
  }

  virtual inline std::pair<Point, Point> hitbox() override;

 private:
  Point drawWhitespace(char whitespace, Point cursor, Point start);
  std::pair<Point, Point> textBox_;
  uint16_t letterSpacing_;
  uint16_t lineSpacing_;
  uint32_t characterScale_;
  Colour<ColourRepresentation> colour_;
  std::optional<Colour<ColourRepresentation>> background_;
  std::string text_;
  Font& font_;
};

template <typename ColourRepresentation>
Point Text<ColourRepresentation>::drawWhitespace(char whitespace, Point cursor,
                                                 Point start) {
  switch (whitespace) {
    case '\n':
      cursor.y_ += (font_.null().sizeY() + lineSpacing_) * characterScale_;
      cursor.x_ = start.x_;
      break;
    case ' ':
      cursor.x_ += letterSpacing_ * characterScale_;
      break;
    default:
      break;
  }
  return cursor;
}

template <typename ColourRepresentation>
void Text<ColourRepresentation>::draw(Screen<ColourRepresentation>& target) {
  ESP_LOGD(TAG_, "Drawing text at (%d, %d)in colour: 0x%X", textBox_.first.x_,
           textBox_.first.y_, colour_.value());
  uint32_t offsetX = textBox_.first.x_;
  uint32_t offsetY = textBox_.first.y_;
  for (const auto& letter : text_) {
    if (isspace(letter)) {
      auto cursor = drawWhitespace(letter, {offsetX, offsetY}, textBox_.first);
      offsetX = cursor.x_;
      offsetY = cursor.y_;
    } else {
      auto bitmap = font_.at(letter);
      if (offsetX + bitmap.sizeX() * characterScale_ >= textBox_.second.x_) {
        offsetY += (bitmap.sizeY() + lineSpacing_) * characterScale_;
        offsetX = textBox_.first.x_;
      }
      if (offsetY + (bitmap.sizeY() + lineSpacing_) * characterScale_ >=
          textBox_.second.y_) {
        break;
      }
      Point newStart(offsetX, offsetY);
      bitmap.drawScaled(target, newStart, colour_, characterScale_,
                        background_);
      offsetX += bitmap.sizeX() + letterSpacing_ * characterScale_;
    }
  }
}

template <typename ColourRepresentation>
std::pair<Point, Point> Text<ColourRepresentation>::hitbox() {
  Point maxPoint;
  return textBox_;
}

}  // namespace EspGL
}  // namespace tamagotchi
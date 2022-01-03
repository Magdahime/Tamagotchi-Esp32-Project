#pragma once

#include <stdint.h>

#include <optional>
#include <string>

#include "../Font/EspGLFont.hpp"
#include "EspGLScreen.hpp"

namespace tamagotchi {
namespace EspGL {

template <typename ColourRepresentation>
class Text {
 public:
  Text(std::string text, Font& font, Colour<ColourRepresentation> colour,
       std::optional<Colour<ColourRepresentation>> background = std::nullopt,
       uint16_t letterSpacing = 5, uint16_t lineSpacing = 2,
       uint32_t characterScale = 3)
      : letterSpacing_(letterSpacing),
        lineSpacing_(lineSpacing),
        characterScale_(characterScale),
        colour_(colour),
        background_(background),
        text_(std::move(text)),
        font_(std::move(font)) {}
  Text(Font& font) : font_(font) {}
  void draw(Screen<ColourRepresentation>& target, Point start);

  inline uint16_t letterSpacing() const { return letterSpacing_; }
  inline uint16_t lineSpacing() const { return lineSpacing_; }
  inline uint32_t characterScale() const { return characterScale_; }
  inline const Colour<ColourRepresentation>& colour() const { return colour_; }
  inline const std::string& text() const { return text_; }
  inline const Font& font() const { return font_; }

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

 private:
  uint16_t letterSpacing_;
  uint16_t lineSpacing_;
  uint32_t characterScale_;
  Colour<ColourRepresentation> colour_;
  std::optional<Colour<ColourRepresentation>> background_;
  std::string text_;
  Font font_;
};

template <typename ColourRepresentation>
void Text<ColourRepresentation>::draw(Screen<ColourRepresentation>& target,
                                      Point start) {
  Point cursor = start;
  for (const auto& letter : text_) {
    auto bitmap = font_.at(letter);
    size_t widthCounter = bitmap.sizeX() * characterScale_;
    for (const auto& bit : bitmap.bitmap()) {
      if (background_ && bit == false) {
        target.screenDriver()->writePixelArea(
            cursor.x_, cursor.x_ + characterScale_, cursor.y_, cursor.y_,
            background_.value().value());
      } else if (bit == true) {
        target.screenDriver()->writePixelArea(
            cursor.x_, cursor.x_ + characterScale_, cursor.y_, cursor.y_,
            colour_.value());
      }
      cursor.x_ += characterScale_;
      widthCounter -= characterScale_;
      if (widthCounter <= 0) {
        cursor.y_ += 1;
        cursor.x_ = 0;
      }
    }
  }
}

}  // namespace EspGL
}  // namespace tamagotchi
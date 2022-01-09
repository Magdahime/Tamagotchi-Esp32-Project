#pragma once

#include <ctype.h>
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
  // void drawLetter(Screen<ColourRepresentation>& target, Bitmap& bitmap,
  //                 Point cursor);
  Point drawWhitespace(char whitespace, Point cursor);
  uint16_t letterSpacing_;
  uint16_t lineSpacing_;
  uint32_t characterScale_;
  Colour<ColourRepresentation> colour_;
  std::optional<Colour<ColourRepresentation>> background_;
  std::string text_;
  Font font_;
};

template <typename ColourRepresentation>
Point Text<ColourRepresentation>::drawWhitespace(char whitespace,
                                                 Point cursor) {
  switch (whitespace) {
    case '\n':
      cursor.y_ += lineSpacing_ * characterScale_;
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
void Text<ColourRepresentation>::draw(Screen<ColourRepresentation>& target,
                                      Point start) {
  ESP_LOGD(TAG_, "Drawing text at (%d, %d)in colour: 0x%X", start.x_, start.y_,
           colour_.value());
  uint32_t offsetX = start.x_;
  uint32_t offsetY = start.y_;
  for (const auto& letter : text_) {
    if (isspace(letter)) {
      auto cursor = drawWhitespace(letter, {offsetX, offsetY});
      offsetX = cursor.x_;
      offsetY = cursor.y_;
    } else {
      auto bitmap = font_.at(letter);
      if (offsetX + bitmap.sizeX() * characterScale_ >= target.width()) {
        offsetY += (bitmap.sizeY() + lineSpacing_) * characterScale_;
        offsetX = start.x_;
      }
      if (offsetY + (bitmap.sizeY() + lineSpacing_) * characterScale_ >=
          target.height()) {
        break;
      }
      Point newStart(offsetX, offsetY);
      bitmap.drawScaled(target, newStart, colour_, characterScale_,
                        background_);
      offsetX += bitmap.sizeX() + letterSpacing_ * characterScale_;
    }
  }
}

// template <typename ColourRepresentation>
// void EspGL::Text<ColourRepresentation>::drawLetter(
//     Screen<ColourRepresentation>& target, Bitmap& bitmap, Point cursor) {
//   uint32_t offset = cursor.x_;
//   size_t widthCounter = bitmap.sizeX() * characterScale_;
//   for (const auto& bit : bitmap.bitmap()) {
//     if (background_ && bit == false) {
//       for (auto i = 0; i < characterScale_; ++i) {
//         target.screenDriver()->writePixelArea(
//             cursor.x_, cursor.x_ + characterScale_, cursor.y_ + i,
//             cursor.y_ + i, background_.value().value());
//       }
//     } else if (bit == true) {
//       for (auto i = 0; i < characterScale_; ++i) {
//         target.screenDriver()->writePixelArea(
//             cursor.x_, cursor.x_ + characterScale_, cursor.y_ + i,
//             cursor.y_ + i, colour_.value());
//       }
//     }
//     cursor.x_ += characterScale_;
//     widthCounter -= characterScale_;
//     if (widthCounter <= 0) {
//       cursor.x_ = offset;
//       cursor.y_ += characterScale_;
//       widthCounter = bitmap.sizeX() * characterScale_;
//     }
//   }
// }

}  // namespace EspGL
}  // namespace tamagotchi
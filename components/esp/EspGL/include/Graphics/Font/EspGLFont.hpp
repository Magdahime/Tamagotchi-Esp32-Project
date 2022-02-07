#pragma once
#include <stdint.h>

#include <map>
#include <string>

#include "../EspGLBitmap.hpp"

namespace tamagotchi {
namespace EspGL {

class Font {
 public:
  Font(uint32_t height, std::map<char, Bitmap>& letters, Bitmap null)
      : fontHeight_(height),
        letters_(std::move(letters)),
        null_(std::move(null)) {}
  Font(uint32_t height, std::map<char, Bitmap>& letters)
      : fontHeight_(height), letters_(std::move(letters)) {}
  Font() = default;
  inline std::map<char, Bitmap>& letters() { return letters_; }
  Bitmap at(char letter) const;
  inline Bitmap null() const { return null_; };
  inline void setNull(Bitmap null) { null_ = null; }
  inline size_t size() { return letters_.size() + (int)!null_.empty(); }
  inline uint32_t height() { return fontHeight_; }

 private:
  uint32_t fontHeight_;
  std::map<char, Bitmap> letters_;
  Bitmap null_;
};

}  // namespace EspGL
}  // namespace tamagotchi
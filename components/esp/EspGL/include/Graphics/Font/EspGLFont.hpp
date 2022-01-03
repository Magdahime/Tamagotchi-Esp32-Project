#pragma once
#include <map>
#include <string>

#include "../EspGLBitmap.hpp"

namespace tamagotchi {
namespace EspGL {

class Font {
 public:
  Font(std::map<char, Bitmap>& letters, Bitmap null)
      : letters_(std::move(letters)), null_(std::move(null)) {}
  Font(std::map<char, Bitmap>& letters) : letters_(std::move(letters)) {}
  inline const std::map<char, Bitmap>& letters() const { return letters_; }
  Bitmap at(char letter) const;
  inline Bitmap null() const { return null_; };
  inline void setNull(Bitmap null) { null_ = null; }
  inline size_t size() { return letters_.size() + (int)!null_.empty(); }

 private:
  std::map<char, Bitmap> letters_;
  Bitmap null_;
};

}  // namespace EspGL
}  // namespace tamagotchi
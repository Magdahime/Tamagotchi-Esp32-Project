#pragma once
#include <stdint.h>

#include <cstdint>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "../EspGLBitmap.hpp"
#include "BitmapLoader.hpp"
#include "EspGLFont.hpp"
#include "EspGLUtils.hpp"

namespace tamagotchi {
namespace EspGL {

class FontLoader {
 public:
  FontLoader(std::fstream fileHandle) : bitmapLoader_(std::move(fileHandle)) {}
  Font load();

 private:
  BitmapLoader::BitmapLoader bitmapLoader_;
};

}  // namespace EspGL
}  // namespace tamagotchi
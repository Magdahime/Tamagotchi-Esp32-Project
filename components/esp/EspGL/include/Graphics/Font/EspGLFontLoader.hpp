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
#include "SPIFFSDriver.hpp"

namespace tamagotchi {
namespace EspGL {

class FontLoader {
 public:
  FontLoader(std::string filename)
      : spiffsDriver_(),
        bitmapLoader_(spiffsDriver_.getFileDescriptor(filename)) {}
  Font load();

 private:
  SPIFFS::SPIFFSDriver spiffsDriver_;
  BitmapLoader::BitmapLoader bitmapLoader_;
};

}  // namespace EspGL
}  // namespace tamagotchi
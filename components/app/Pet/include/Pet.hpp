#pragma once

#include <stdint.h>

#include <array>
#include <string>

#include "PetCommon.hpp"
#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Graphics/EspGLBitmap.hpp"
#include "Graphics/EspGLPicture.hpp"

namespace tamagotchi {
namespace App {

namespace Pet {


template <typename ColourRepresentation>
class Pet {
 public:
  Pet(std::string name) : name_(name) {
    needs_.resize(consts::PET_NEEDS);
    std::fill(needs_.begin(), needs_.end(), consts::MAX_NEED_VALUE);
  }
  Pet() = default;
  virtual ~Pet() = default;

  void giveFood();
  void giveSnack();
  void play();

  inline std::string& name() { return name_; }
  inline std::vector<int16_t>& needs() { return needs_; }

 protected:
  std::string name_;
  std::vector<int16_t> needs_;
};

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

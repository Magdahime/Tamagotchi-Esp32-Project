#pragma once

#include <stdint.h>

#include <optional>
#include <variant>

#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "EspGLVect2.hpp"
#include "GomokuCommon.hpp"
#include "GomokuNetworking.hpp"
#include "GomokuNetworkingConf.hpp"
#include "Shapes/EspGLRectangles.hpp"
namespace tamagotchi::App::Gomoku {

template <typename ColourRepresentation>
class GomokuTile {
 public:
  GomokuTile() = default;
  GomokuTile(EspGL::Vect2 leftUpper, EspGL::Vect2 rightLower)
      : leftUpperCanvas_(leftUpper), rightLowerCanvas_(rightLower) {}

  GomokuTile(EspGL::Hitbox canvas)
      : leftUpperCanvas_(canvas.first), rightLowerCanvas_(canvas.second) {}

  ~GomokuTile() = default;

  bool empty() { return std::holds_alternative<std::monostate>(playerId_); }

  void mark();
  void getMark();

  void highlight() { highlighted_ = true; }
  void dehighlight() { highlighted_ = false; }
  bool highlighted() { return highlighted_; }
  std::variant<std::monostate, uint8_t, GomokuNetworking::mac_address_t>&
  playerId() {
    return playerId_;
  }

  EspGL::Hitbox hitbox() {
    return EspGL::Hitbox(leftUpperCanvas_, rightLowerCanvas_);
  }

  void setHitbox(EspGL::Hitbox newHitbox) {
    leftUpperCanvas_ = newHitbox.first;
    rightLowerCanvas_ = newHitbox.second;
  }

  inline const EspGL::Vect2& leftUpperCanvas() const {
    return leftUpperCanvas_;
  }

  inline const EspGL::Vect2& rightLowerCanvas() const {
    return rightLowerCanvas_;
  }

 private:
  bool highlighted_;
  EspGL::Vect2 leftUpperCanvas_;
  EspGL::Vect2 rightLowerCanvas_;
  std::variant<std::monostate, uint8_t, GomokuNetworking::mac_address_t>
      playerId_;
};

}  // namespace tamagotchi::App::Gomoku
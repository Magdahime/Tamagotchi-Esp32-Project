#pragma once

#include <stdint.h>

#include "Drawable.hpp"
#include "EspGLVect2.hpp"
#include "GomokuCommon.hpp"
#include "GomokuNetworking.hpp"
#include "GomokuNetworkingConf.hpp"
namespace tamagotchi::App::Gomoku {

template <typename ColourRepresentation>
class GomokuTile : public EspGL::Drawable<ColourRepresentation> {
 public:
  GomokuTile() = default;
  GomokuTile(EspGL::Vect2 leftUpper, EspGL::Vect2 rightLower)
      : sign_(0),
        playerAddress_(0),
        leftUpperCanvas_(leftUpper),
        rightLowerCanvas_(rightLower) {}

  GomokuTile(EspGL::Hitbox canvas)
      : leftUpperCanvas_(canvas.first), rightLowerCanvas_(canvas.second) {}

  ~GomokuTile() = default;

  void mark();
  void getMark();

  void highlight() { highlighted_ = true; }
  void dehighlight() { highlighted_ = false; }

  EspGL::Hitbox hitbox() override {
    return EspGL::Hitbox(leftUpperCanvas_, rightLowerCanvas_);
  }

  inline const EspGL::Vect2& leftUpperCanvas() const {
    return leftUpperCanvas_;
  }

  inline const EspGL::Vect2& rightLowerCanvas() const {
    return rightLowerCanvas_;
  }

  void draw(EspGL::Screen<ColourRepresentation>& target) override;

 private:
  bool highlighted_;
  EspGL::Vect2 leftUpperCanvas_;
  EspGL::Vect2 rightLowerCanvas_;
  uint8_t sign_;
  GomokuNetworking::mac_address_t playerAddress_;
};

template <typename ColourRepresentation>
void GomokuTile<ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation>& target) {}

}  // namespace tamagotchi::App::Gomoku
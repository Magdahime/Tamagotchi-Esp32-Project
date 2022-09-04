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

  ~GomokuTile() = default;

  bool empty() { return std::holds_alternative<std::monostate>(playerId_); }

  void mark();
  void getMark();

  void highlight() {
    if (previouslyHighlighted_) previouslyHighlighted_ = false;
    highlighted_ = true;
  }
  void dehighlight() {
    if (highlighted_) previouslyHighlighted_ = true;
    highlighted_ = false;
  }
  bool highlighted() const { return highlighted_; }
  bool previouslyHighlighted() const { return previouslyHighlighted_; }

  void setPreviouslyHighlighted(bool newValue) {
    previouslyHighlighted_ = newValue;
  }

  std::variant<std::monostate, uint8_t, GomokuNetworking::mac_address_t>&
  playerId() {
    return playerId_;
  }

  void clear() {
    highlighted_ = false;
    previouslyHighlighted_ = false;
    playerId_ = std::monostate{};
  }

 private:
  bool highlighted_;
  bool previouslyHighlighted_;
  std::variant<std::monostate, uint8_t, GomokuNetworking::mac_address_t>
      playerId_;
};

}  // namespace tamagotchi::App::Gomoku
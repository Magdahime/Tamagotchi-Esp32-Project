#pragma once

#include "GomokuCommon.hpp"

namespace tamagotchi::App::Gomoku {

template <unsigned width_s, unsigned height_s, typename PlayerIdentifier>
class GomokuBase {
 public:
  GomokuBase() = default;
  bool isWinner() { return winner_ != 0 ? true : false; }
  PlayerIdentifier winner() { return winner_; }
  constexpr unsigned width() const { return width_s; }
  constexpr unsigned height() const { return height_s; }

  virtual PlayerIdentifier markMove(
      PlayerIdentifier playerId, GomokuNetworking::BoardCoordinate& move) = 0;

 protected:
  PlayerIdentifier winner_;
  virtual PlayerIdentifier checkWinner(
      PlayerIdentifier playerId, GomokuNetworking::BoardCoordinate& move) = 0;
};

}  // namespace tamagotchi::App::Gomoku
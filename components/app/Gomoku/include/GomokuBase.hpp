#pragma once

#include "GomokuCommon.hpp"

namespace tamagotchi::App::Gomoku {

template <unsigned width_s, unsigned height_s>
class GomokuBase {
 public:
  GomokuBase() : winner_(0) {}
  bool isWinner() { return winner_ != 0 ? true : false; }
  uint8_t winner() { return winner_; }
  constexpr unsigned width() const { return width_s; }
  constexpr unsigned height() const { return height_s; }
  
 protected:
  uint8_t winner_;
  virtual uint8_t checkWinner(uint8_t playerSign,
                              GomokuNetworking::BoardCoordinate& move) = 0;
};

}  // namespace tamagotchi::App::Gomoku
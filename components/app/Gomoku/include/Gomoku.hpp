#pragma once

#include <stdint.h>
#include <stdio.h>

#include <array>
#include <map>
#include <stdexcept>
#include <string>

#include "GomokuNetworking.hpp"
#include "GomokuNetworkingConf.hpp"

namespace tamagotchi::App::Gomoku {

constexpr int POINT_COUNT = 3;
constexpr int INITIAL_VALUE = 0;

template <unsigned width_s, unsigned height_s>
class Gomoku {
 public:
  Gomoku() : winner_(0) { board_.fill(INITIAL_VALUE); }
  bool isWinner() { return winner_ != 0 ? true : false; }
  uint8_t winner() { return winner_; }
  mac_address_t macWinner();

  inline uint8_t player2Int(mac_address_t mac) { return player2Int_.at(mac); }
  inline std::array<uint8_t, width_s * height_s>& board() { return board_; }
  constexpr unsigned width() const { return width_s; }
  constexpr unsigned height() const { return height_s; }
  uint8_t markMove(uint8_t playerSign, BoardCoordinate& move);

  void fillPlayers(const std::vector<mac_address_t>& macAddresses);

 protected:
  uint8_t winner_;
  static constexpr char TAG_[] = "Gomoku";
  std::array<uint8_t, width_s * height_s> board_;

  std::map<mac_address_t, uint8_t> player2Int_;
  uint8_t checkWinner(uint8_t playerSign, BoardCoordinate& move);
};

template <unsigned width_s, unsigned height_s>
uint8_t Gomoku<width_s, height_s>::markMove(uint8_t playerSign,
                                            BoardCoordinate& move) {
  if (playerSign == 0) {
    throw std::runtime_error("Marking playerSign shouldn't be zero!");
  }
  if (move.x_ * move.y_ > width_s * height_s) {
    throw std::runtime_error("Move coordinates are out of bounds: (" +
                             std::to_string(move.x_) + ", " +
                             std::to_string(move.y_) + ")");
  }
  if (board_[move.y_ * width_s + move.x_] != INITIAL_VALUE) {
    throw std::runtime_error("This tile has been already used: (" +
                             std::to_string(move.x_) + ", " +
                             std::to_string(move.y_) + ")");
  }
  board_[move.y_ * width_s + move.x_] = playerSign;
  return checkWinner(playerSign, move);
}

template <unsigned width_s, unsigned height_s>
uint8_t Gomoku<width_s, height_s>::checkWinner(uint8_t playerSign,
                                               BoardCoordinate& move) {
  auto calculateArrayCoord = [&](int16_t x, int16_t y) {
    return y * width_s + x;
  };

  auto checkLoop = [&](int16_t x, int16_t y, int16_t stepX, int16_t stepY,
                       uint16_t lineLength) {
    uint8_t counter = 0;
    int range = lineLength - 1;
    int beginX =
        x - range * stepX >= 0
            ? (x - range * stepX > width_s ? width_s - 1 : x - range * stepX)
            : 0;
    int beginY =
        y - range * stepY >= 0
            ? (y - range * stepY > height_s ? height_s - 1 : y - range * stepY)
            : 0;
    int endX = x + range * stepX >= width_s
                   ? width_s - 1
                   : (x + range * stepX >= 0 ? x + range * stepX : 0);
    int endY = y + range * stepY >= height_s
                   ? height_s - 1
                   : (y + range * stepY >= 0 ? y + range * stepY : 0);
    auto compare = [&](auto coords) {
      if (beginX == endX) {
        return coords.second <= endY;
      }
      if (beginY == endY) {
        return coords.first <= endX;
      }
      if (beginY != endY && beginX != endX) {
        if (stepX > 0) {
          return (coords.first <= endX) && (coords.second <= endY);
        } else {
          return (coords.first >= endX) && (coords.second <= endY);
        }
      }
      return false;
    };
    for (auto coords = std::make_pair(beginX, beginY); compare(coords);
         coords.first += stepX, coords.second += stepY) {
      if (board_[calculateArrayCoord(coords.first, coords.second)] ==
          playerSign) {
        counter++;
      } else {
        counter = 0;
      }
      if (counter == lineLength) {
        return true;
      }
    }
    return false;
  };

  if (checkLoop(move.x_, move.y_, 1, 0, POINT_COUNT) ||
      checkLoop(move.x_, move.y_, 0, 1, POINT_COUNT) ||
      checkLoop(move.x_, move.y_, 1, 1, POINT_COUNT) ||
      checkLoop(move.x_, move.y_, -1, 1, POINT_COUNT)) {
    return playerSign;
  }
  return 0;
}

template <unsigned width_s, unsigned height_s>
void Gomoku<width_s, height_s>::fillPlayers(
    const std::vector<mac_address_t>& macAddresses) {
  player2Int_.emplace(GomokuNetworking::hostAddress(), 1);
  int sign = 2;
  for (const auto& mac : macAddresses) {
    player2Int_.emplace(mac, sign);
    sign++;
  }
}

template <unsigned width_s, unsigned height_s>
mac_address_t Gomoku<width_s, height_s>::macWinner() {
  auto findResult =
      std::find_if(player2Int_.begin(), player2Int_.end(),
                   [&](const auto& pair) { return pair.second == winner_; });

  if (findResult == player2Int_.end()) {
    return {};
  }
  return findResult->first;
}

}  // namespace tamagotchi::App::Gomoku

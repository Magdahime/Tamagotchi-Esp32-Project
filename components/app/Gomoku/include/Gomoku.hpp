#pragma once

#include <stdint.h>
#include <stdio.h>

#include <array>
#include <stdexcept>
#include <string>

#include "GomokuNetworking.hpp"

namespace tamagotchi {

namespace App {

namespace Gomoku {

constexpr int POINT_COUNT = 3;
constexpr int INITIAL_VALUE = 0;

struct BoardCoordinate {
  int16_t x_;
  int16_t y_;
};

template <unsigned width_s, unsigned height_s>
class Gomoku {
 public:
  Gomoku() { board_.fill(INITIAL_VALUE); }
  uint8_t checkWinner(uint8_t playerSign, const BoardCoordinate& move) const;
  inline std::array<uint8_t, width_s * height_s>& board() { return board_; }
  constexpr unsigned width() const { return width_s; }
  constexpr unsigned height() const { return height_s; }
  uint8_t markMove(uint8_t playerSign, const BoardCoordinate& move);

 protected:
  static constexpr char TAG_[] = "Gomoku";
  std::array<uint8_t, width_s * height_s> board_;
};

template <unsigned width_s, unsigned height_s>
uint8_t Gomoku<width_s, height_s>::markMove(uint8_t playerSign,
                                            const BoardCoordinate& move) {
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
uint8_t Gomoku<width_s, height_s>::checkWinner(
    uint8_t playerSign, const BoardCoordinate& move) const {
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

}  // namespace Gomoku

}  // namespace App

}  // namespace tamagotchi

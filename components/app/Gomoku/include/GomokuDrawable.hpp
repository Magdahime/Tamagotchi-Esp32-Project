#pragma once

#include <stdint.h>

#include <map>
#include <utility>
#include <vector>

#include "Drawable.hpp"
#include "EspGLVect2.hpp"
#include "Globals.hpp"
#include "GomokuBase.hpp"
#include "GomokuCommon.hpp"
#include "GomokuTile.hpp"
#include "Shapes/EspGLRectangles.hpp"
#include "Shapes/EspGLSmoothEdgeShapes.hpp"

namespace tamagotchi {
namespace App {
namespace Gomoku {

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
class GomokuDrawable
    : public EspGL::Drawable<ColourRepresentation>,
      public GomokuBase<width_s, height_s, GomokuNetworking::mac_address_t> {
 public:
  GomokuDrawable(EspGL::Vect2 start, EspGL::Vect2 end)
      : leftUpperCanvas_(start), rightLowerCanvas_(end) {
    calculateTileHitboxes();
  }

  GomokuDrawable(EspGL::Hitbox canvas)
      : leftUpperCanvas_(canvas.first), rightLowerCanvas_(canvas.second) {
    calculateTileHitboxes();
  }

  ~GomokuDrawable() = default;

  void draw(EspGL::Screen<ColourRepresentation>& target) override;

  uint8_t update(EspGL::Screen<ColourRepresentation>& target,
                 GomokuNetworking::PlayerMove nextMove);

  inline EspGL::Hitbox hitbox() override {
    return EspGL::Hitbox(leftUpperCanvas_, rightLowerCanvas_);
  }

  inline const EspGL::Vect2& leftUpperCanvas() const {
    return leftUpperCanvas_;
  }

  inline const EspGL::Vect2& rightLowerCanvas() const {
    return rightLowerCanvas_;
  }

  std::map<GomokuNetworking::mac_address_t,
           EspGL::Colour<ColourRepresentation>>&
  player2Colour() {
    return player2Colour_;
  }

  std::array<GomokuTile<ColourRepresentation>, width_s * height_s>& board() {
    return board_;
  }

  virtual GomokuNetworking::mac_address_t markMove(
      GomokuNetworking::mac_address_t playerId,
      GomokuNetworking::BoardCoordinate& move) override;

  bool isWinner() { return winner_.has_value(); }

 private:
  std::optional<GomokuNetworking::mac_address_t> winner_;
  static constexpr char TAG_[] = "GomokuDrawable";
  EspGL::Vect2 leftUpperCanvas_;
  EspGL::Vect2 rightLowerCanvas_;
  std::array<GomokuTile<ColourRepresentation>, width_s * height_s> board_;

  void drawTile(GomokuTile<ColourRepresentation>& tile,
                EspGL::Screen<ColourRepresentation>& target);

  std::map<GomokuNetworking::mac_address_t, EspGL::Colour<ColourRepresentation>>
      player2Colour_;

  virtual GomokuNetworking::mac_address_t checkWinner(
      GomokuNetworking::mac_address_t playerId,
      GomokuNetworking::BoardCoordinate& move) override;

  void calculateTileHitboxes();
};

/////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////////////////////

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
void GomokuDrawable<width_s, height_s, ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation>& target) {
  auto width = rightLowerCanvas_.x_ - leftUpperCanvas_.x_;
  auto height = rightLowerCanvas_.y_ - leftUpperCanvas_.y_;

  auto drawRectangle = [=, &target](
                           EspGL::Vect2 start, int16_t sizeX, int16_t sizeY,
                           EspGL::Colour<ColourRepresentation> colour) {
    EspGL::Rectangle<ColourRepresentation>{start, sizeX, sizeY, colour}.draw(
        target);
  };

  EspGL::Vect2 start{width / width_s, leftUpperCanvas_.y_};

  for (auto i = 0; i < width_s - 1; i++) {
    drawRectangle(start, consts::GOMOKU_LINE_WIDTH, height - 1,
                  consts::LINE_COLOUR);
    start.x_ += consts::GOMOKU_LINE_WIDTH + width / width_s;
  }
  start = {leftUpperCanvas_.x_, height / height_s};
  for (auto i = 0; i < height_s - 1; i++) {
    drawRectangle(start, width - 1, consts::GOMOKU_LINE_WIDTH,
                  consts::LINE_COLOUR);
    start.y_ += consts::GOMOKU_LINE_WIDTH + height / height_s;
  }

  for (auto& tile : board_) {
    drawTile(tile, target);
  }
}

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
GomokuNetworking::mac_address_t
GomokuDrawable<width_s, height_s, ColourRepresentation>::markMove(
    GomokuNetworking::mac_address_t playerMac,
    GomokuNetworking::BoardCoordinate& move) {
  if (move.y_ * width_s + move.x_ > width_s * height_s) {
    throw std::runtime_error("Move coordinates are out of bounds: (" +
                             std::to_string(move.x_) + ", " +
                             std::to_string(move.y_) + ")");
  }
  if (!board_[move.y_ * width_s + move.x_].empty()) {
    throw std::runtime_error("This tile has been already used: (" +
                             std::to_string(move.x_) + ", " +
                             std::to_string(move.y_) + ")");
  }
  board_[move.y_ * width_s + move.x_].playerId() = playerMac;
  return checkWinner(playerMac, move);
}

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
GomokuNetworking::mac_address_t
GomokuDrawable<width_s, height_s, ColourRepresentation>::checkWinner(
    GomokuNetworking::mac_address_t playerId,
    GomokuNetworking::BoardCoordinate& move) {
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
      if (std::get<GomokuNetworking::mac_address_t>(
              board_[calculateArrayCoord(coords.first, coords.second)]
                  .playerId()) == playerId) {
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

  if (checkLoop(move.x_, move.y_, 1, 0, consts::POINT_COUNT) ||
      checkLoop(move.x_, move.y_, 0, 1, consts::POINT_COUNT) ||
      checkLoop(move.x_, move.y_, 1, 1, consts::POINT_COUNT) ||
      checkLoop(move.x_, move.y_, -1, 1, consts::POINT_COUNT)) {
    return playerId;
  }
  return {};
}

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
void GomokuDrawable<width_s, height_s, ColourRepresentation>::drawTile(
    GomokuTile<ColourRepresentation>& tile,
    EspGL::Screen<ColourRepresentation>& target) {
  auto hitbox = tile.hitbox();
  ESP_LOGI("GomokuDrawable", "TILE HITBOX: (%d, %d) (%d, %d)", hitbox.first.x_,
           hitbox.first.y_, hitbox.second.x_, hitbox.second.y_);
  if (tile.highlighted()) {
    EspGL::RectangleOutline<uint16_t>{
        hitbox.first,
        static_cast<int16_t>(hitbox.second.x_ - hitbox.first.x_ - 2),
        static_cast<int16_t>(hitbox.second.y_ - hitbox.first.y_ - 2),
        consts::HIGHLIGHT_COLOUR}
        .draw(target);
  }

  if (!tile.empty()) {
    EspGL::Circle<ColourRepresentation>{
        EspGL::Vect2(hitbox.second.x_ / 2, hitbox.second.y_ / 2),
        consts::GOMOKU_PAWN_SIZE,
        player2Colour_.at(
            std::get<GomokuNetworking::mac_address_t>(tile.playerId()))}
        .draw(target);
  }
}

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
void GomokuDrawable<width_s, height_s,
                    ColourRepresentation>::calculateTileHitboxes() {
  const auto width = rightLowerCanvas_.x_ - leftUpperCanvas_.x_;
  const auto height = rightLowerCanvas_.y_ - leftUpperCanvas_.y_;
  auto diffX = width / width_s;
  auto diffY = height / height_s;
  auto startLeftUpper =
      EspGL::Vect2(leftUpperCanvas_.x_ - 1, leftUpperCanvas_.y_ - 1);
  auto startRightLower = EspGL::Vect2(startLeftUpper.x_ + diffX - 1,
                                      startLeftUpper.y_ + diffY - 1);
  auto currentTile = 0;
  for (auto y = 0u; y < height_s; y++) {
    for (auto x = 0u; x < width_s; x++) {
      board_[currentTile].setHitbox(
          EspGL::Hitbox{startLeftUpper, startRightLower});
      currentTile++;
      startLeftUpper.x_ += diffX;
      startRightLower.x_ += diffX;
    }
    startLeftUpper = EspGL::Vect2(0, startRightLower.y_);
    startRightLower =
        EspGL::Vect2(startLeftUpper.x_ + diffX, startLeftUpper.y_ + diffY);
  }
}

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi
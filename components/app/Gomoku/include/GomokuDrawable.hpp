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
      : leftUpperCanvas_(start), rightLowerCanvas_(end) {}

  GomokuDrawable(EspGL::Hitbox canvas)
      : leftUpperCanvas_(canvas.first), rightLowerCanvas_(canvas.second) {}

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

  bool full() {
    return std::all_of(board_.begin(), board_.end(),
                       [](GomokuTile<ColourRepresentation> tile) {
                         return tile.empty() == false;
                       });
  }

 private:
  static constexpr char TAG_[] = "GomokuDrawable";
  EspGL::Vect2 leftUpperCanvas_;
  EspGL::Vect2 rightLowerCanvas_;
  std::array<GomokuTile<ColourRepresentation>, width_s * height_s> board_;

  void drawTile(int tileIndex, EspGL::Screen<ColourRepresentation>& target);

  std::map<GomokuNetworking::mac_address_t, EspGL::Colour<ColourRepresentation>>
      player2Colour_;

  virtual GomokuNetworking::mac_address_t checkWinner(
      GomokuNetworking::mac_address_t playerId,
      GomokuNetworking::BoardCoordinate& move) override;

  EspGL::Hitbox calculateTileHitbox(int tileIndex);
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

  const auto diffX = width / width_s;
  const auto diffY = height / height_s;

  EspGL::Vect2 gridLineStart{
      leftUpperCanvas_.x_ + diffX - consts::GOMOKU_LINE_WIDTH / 2,
      leftUpperCanvas_.y_};

  for (auto i = 0; i < width_s - 1; i++) {
    drawRectangle(gridLineStart, consts::GOMOKU_LINE_WIDTH, height,
                  consts::LINE_COLOUR);
    gridLineStart.x_ += diffX;
  }
  gridLineStart = {leftUpperCanvas_.x_,
                   leftUpperCanvas_.y_ + diffY - consts::GOMOKU_LINE_WIDTH / 2};
  for (auto i = 0; i < height_s - 1; i++) {
    drawRectangle(gridLineStart, width, consts::GOMOKU_LINE_WIDTH,
                  consts::LINE_COLOUR);
    gridLineStart.y_ += diffY;
  }

  for (auto tileIndex = 0; tileIndex < board_.size(); tileIndex++) {
    drawTile(tileIndex, target);
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
  if (!board_[move.y_ * width_s + move.x_].empty() &&
      playerMac != std::get<GomokuNetworking::mac_address_t>(
                       board_[move.y_ * width_s + move.x_].playerId())) {
    throw std::runtime_error("This tile has been already used: (" +
                             std::to_string(move.x_) + ", " +
                             std::to_string(move.y_) + ")");
  } else if (board_[move.y_ * width_s + move.x_].empty()) {
    board_[move.y_ * width_s + move.x_].playerId() = playerMac;
  }
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
      auto variant =
          board_[calculateArrayCoord(coords.first, coords.second)].playerId();
      if (std::holds_alternative<GomokuNetworking::mac_address_t>(variant) &&
          std::get<GomokuNetworking::mac_address_t>(variant) == playerId) {
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
    ESP_LOGI("TAG_", "playerId: " MACSTR, MAC2STR(playerId));
    this->winner_ = playerId;
    ESP_LOGI("TAG_", "winner: " MACSTR, MAC2STR((this->winner_.value())));
    return this->winner_.value();
  }
  return {};
}

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
void GomokuDrawable<width_s, height_s, ColourRepresentation>::drawTile(
    int tileIndex, EspGL::Screen<ColourRepresentation>& target) {
  auto& tile = board_[tileIndex];
  auto hitbox = calculateTileHitbox(tileIndex);
  if (tile.highlighted()) {
    EspGL::Rectangle<uint16_t>{
        hitbox.first, static_cast<int16_t>(hitbox.second.x_ - hitbox.first.x_),
        static_cast<int16_t>(hitbox.second.y_ - hitbox.first.y_),
        consts::HIGHLIGHT_COLOUR}
        .draw(target);
  }

  if (tile.previouslyHighlighted()) {
    EspGL::Rectangle<uint16_t>(
        hitbox.first, static_cast<int16_t>(hitbox.second.x_ - hitbox.first.x_),
        static_cast<int16_t>(hitbox.second.y_ - hitbox.first.y_),
        Globals::defaultValues::BACKGROUND_COLOUR)
        .draw(target);
    tile.setPreviouslyHighlighted(false);
  }

  if (!tile.empty()) {
    auto diffX = (hitbox.second.x_ - hitbox.first.x_) / 2;
    auto diffY = (hitbox.second.y_ - hitbox.first.y_) / 2;
    auto middlePoint =
        EspGL::Vect2(hitbox.first.x_ + diffX, hitbox.first.y_ + diffY);
    EspGL::Circle<ColourRepresentation>{
        middlePoint, consts::GOMOKU_PAWN_SIZE,
        player2Colour_.at(
            std::get<GomokuNetworking::mac_address_t>(tile.playerId()))}
        .draw(target);
  }
}

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
EspGL::Hitbox
GomokuDrawable<width_s, height_s, ColourRepresentation>::calculateTileHitbox(
    int tileIndex) {
  const auto width = rightLowerCanvas_.x_ - leftUpperCanvas_.x_;
  const auto height = rightLowerCanvas_.y_ - leftUpperCanvas_.y_;
  const auto diffX = width / width_s;
  const auto diffY = height / height_s;
  const auto row = tileIndex / width_s;
  const auto column = tileIndex % width_s;
  const auto leftUpper = EspGL::Vect2(
      leftUpperCanvas_.x_ + diffX * column + consts::GOMOKU_LINE_WIDTH,
      leftUpperCanvas_.y_ + diffY * row + consts::GOMOKU_LINE_WIDTH);
  const auto rightLower =
      EspGL::Vect2(leftUpper.x_ + diffX - 2 * consts::GOMOKU_LINE_WIDTH,
                   leftUpper.y_ + diffY - 2 * consts::GOMOKU_LINE_WIDTH);
  return EspGL::Hitbox(std::move(leftUpper), std::move(rightLower));
}

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi
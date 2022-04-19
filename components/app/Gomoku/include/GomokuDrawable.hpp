#pragma once

#include <stdint.h>

#include <map>
#include <utility>
#include <vector>

#include "Drawable.hpp"
#include "EspGLVect2.hpp"
#include "Globals.hpp"
#include "Gomoku.hpp"
#include "Shapes/EspGLRectangles.hpp"
#include "Shapes/EspGLSmoothEdgeShapes.hpp"

namespace tamagotchi {
namespace App {
namespace Gomoku {

using PlayerMove = std::pair<uint8_t, BoardCoordinate>;

namespace consts {
constexpr int GOMOKU_PAWN_SIZE = 25;  // in pixels
constexpr int GOMOKU_LINE_WIDTH = 2;  // in pixels
constexpr auto LINE_COLOUR = EspGL::colours::WHITE;
}  // namespace consts

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
class GomokuDrawable : public EspGL::Drawable<ColourRepresentation> {
 public:
  GomokuDrawable(EspGL::Vect2 start, EspGL::Vect2 end)
      : leftUpperCanvas_(start), rightLowerCanvas_(end) {}

  GomokuDrawable(EspGL::Hitbox canvas)
      : leftUpperCanvas_(canvas.first), rightLowerCanvas_(canvas.second) {}

  ~GomokuDrawable() = default;

  void draw(EspGL::Screen<ColourRepresentation>& target) override;
  uint8_t update(EspGL::Screen<ColourRepresentation>& target,
                 PlayerMove nextMove);
  EspGL::Hitbox hitbox() override {
    return EspGL::Hitbox(leftUpperCanvas_, rightLowerCanvas_);
  }

  const Gomoku<width_s, height_s>& gomokuBoard() { return gomokuBoard_; }

  inline const EspGL::Vect2& leftUpperCanvas() const {
    return leftUpperCanvas_;
  }

  inline const EspGL::Vect2& rightLowerCanvas() const {
    return rightLowerCanvas_;
  }

 private:
  EspGL::Vect2 leftUpperCanvas_;
  EspGL::Vect2 rightLowerCanvas_;
  std::vector<EspGL::Hitbox> cellHitboxes_;
  std::map<uint8_t, EspGL::Colour<ColourRepresentation>> player2Colour_;
  Gomoku<width_s, height_s> gomokuBoard_;
};

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
void GomokuDrawable<width_s, height_s, ColourRepresentation>::draw(
    EspGL::Screen<ColourRepresentation>& target) {
  auto width = target.width();
  auto height = target.height();

  auto drawRectangle = [=, &target](
                           EspGL::Vect2 start, int16_t sizeX, int16_t sizeY,
                           EspGL::Colour<ColourRepresentation> colour) {
    EspGL::Rectangle<ColourRepresentation>{start, sizeX, sizeY, colour}.draw(
        target);
  };

  EspGL::Vect2 start{width / width_s, 0};

  for (auto i = 0; i < width_s - 1; i++) {
    drawRectangle(start, consts::GOMOKU_LINE_WIDTH, height - 1,
                  consts::LINE_COLOUR);
    start.x_ += consts::GOMOKU_LINE_WIDTH + width / width_s;
  }
  start = {0, height / height_s};
  for (auto i = 0; i < height_s - 1; i++) {
    drawRectangle(start, width - 1, consts::GOMOKU_LINE_WIDTH,
                  consts::LINE_COLOUR);
    start.y_ += consts::GOMOKU_LINE_WIDTH + height / height_s;
  }
}

template <unsigned width_s, unsigned height_s, typename ColourRepresentation>
uint8_t GomokuDrawable<width_s, height_s, ColourRepresentation>::update(
    EspGL::Screen<ColourRepresentation>& target, PlayerMove nextMove) {
  auto result = gomokuBoard_.markMove(nextMove.first, nextMove.second);
  auto cellHitbox =
      cellHitboxes_[nextMove.second.x_ + width_s * nextMove.second.y_];
  EspGL::Circle<ColourRepresentation>{
      EspGL::Vect2(cellHitbox.second.x_ / 2, cellHitbox.second.y_ / 2),
      consts::GOMOKU_PAWN_SIZE, player2Colour_.at(nextMove.first)}
      .draw(target);
  return result;
}

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi
#pragma once
#include <stdint.h>

#include "Event.hpp"
#include "GomokuDrawable.hpp"
#include "State.hpp"
#include "StateUtils.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class MiniGameState : public State {
 public:
  MiniGameState();
  virtual ~MiniGameState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;
  virtual std::string toString() override { return TAG_; }

 private:
  Gomoku::GomokuDrawable<consts::GOMOKU_WIDTH, consts::GOMOKU_HEIGHT, uint16_t>
      gomokuBoard_;
  std::vector<uint8_t> players_;
  std::vector<Gomoku::PlayerMove> evaluateEvents();
  void updateBoard(std::vector<Gomoku::PlayerMove> moves);
  void printWinner(uint8_t winner);
  static constexpr char TAG_[] = "MiniGameState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
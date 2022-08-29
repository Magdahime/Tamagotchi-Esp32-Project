#pragma once
#include "Game.hpp"
#include "GomokuDrawable.hpp"
#include "GomokuNetworking.hpp"
#include "State.hpp"
#include "StateUtils.hpp"

namespace tamagotchi::App::State {

class PlayerTurnState : public State {
 public:
  PlayerTurnState();
  virtual ~PlayerTurnState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void stateInit() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  int16_t rowCounter_;
  int16_t columnCounter_;
  Gomoku::GomokuDrawable<Game::consts::GOMOKU_WIDTH,
                         Game::consts::GOMOKU_HEIGHT, uint16_t>& gomokuBoard_;
  void redrawScreen();
  void handleGpioInput(gpio_num_t pressedButton);
  void sendMoveUpdateToHost(GomokuNetworking::BoardCoordinate move);
  static constexpr char TAG_[] = "PlayerTurnState";
};

}  // namespace tamagotchi::App::State
#include "MiniGameState.hpp"

#include "Game.hpp"
#include "Globals.hpp"

namespace tamagotchi {
namespace App {
namespace State {

MiniGameState::MiniGameState()
    : gomokuBoard_(EspGL::Vect2(0, 0),
                   EspGL::Vect2(Globals::game.screen().width(),
                                Globals::game.screen().height())) {}

void MiniGameState::handleEvent(Event::Event event) {}

void MiniGameState::init() {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  gomokuBoard_.draw(Globals::game.screen());
}

void MiniGameState::mainLoop() {
  auto moves = evaluateEvents();
  updateBoard(moves);
  EspGL::delay(4000);
}

std::vector<Gomoku::PlayerMove> MiniGameState::evaluateEvents() { return {}; }
void MiniGameState::updateBoard(std::vector<Gomoku::PlayerMove> moves) {
  for (const auto& move : moves) {
    if (gomokuBoard_.update(Globals::game.screen(), move) != 0) {
      printWinner(move.first);
      return;
    }
  }
}
void MiniGameState::printWinner(uint8_t winner) {
  Globals::game.print(
      "###### Player " + std::to_string(winner) +
          " wins!\n######\nCONGRATULATIONS !",
      {{0, 50},
       {Globals::game.screen().width(), Globals::game.screen().height()}},
      EspGL::colours::GREEN);
  EspGL::delay(4000);
  Globals::game.setNextState(StateType::MainMenu);
}

void MiniGameState::deinit() {}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
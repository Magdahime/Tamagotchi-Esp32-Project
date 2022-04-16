#include "MiniGameState.hpp"

#include "Game.hpp"
#include "Globals.hpp"

namespace tamagotchi {
namespace App {
namespace State {

MiniGameState::MiniGameState() { gomokuBoard_.draw(Globals::game.screen());}

void MiniGameState::handleEvent(Event::Event event) {}

void MiniGameState::init() {}

void MiniGameState::mainLoop() {
  evaluateEvents();
  updateBoard();
  EspGL::delay(4000);
}

void MiniGameState::evaluateEvents() {}
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
      "######\Player " + winner + " wins!\n######\nCONGRATULATIONS !",
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
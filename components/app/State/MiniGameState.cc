#include "MiniGameState.hpp"

#include "Game.hpp"
#include "Globals.hpp"

namespace tamagotchi {
namespace App {
namespace State {

MiniGameState::MiniGameState() {}

void MiniGameState::handleEvent(Event::Event event) {}

void MiniGameState::init() {}

void MiniGameState::mainLoop() {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "###########\nUNDER CONSTRUCTION\n###########",
      {{0, 0},
       {Globals::game.screen().width(), Globals::game.screen().height()}},
      EspGL::colours::GREEN);
  EspGL::delay(4000);
  Globals::game.setNextState(StateType::MainMenu);
}

void MiniGameState::deinit() {}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
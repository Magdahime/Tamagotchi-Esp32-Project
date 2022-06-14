#include "State.hpp"

#include "Game.hpp"
#include "Globals.hpp"

namespace tamagotchi {
namespace App {
namespace State {
void State::run() {
  init();
  loop();
  deinit();
  Globals::game.shiftState();
}
void State::loop() {
  Globals::game.eventQueue().clearQueue();
  while (Globals::game.currentState() == Globals::game.nextState()) {
    mainLoop();
  }
}

void State::displayErrorMessage(std::string message, EspGL::Vect2 placing) {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      message,
      {placing,
       {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      Game::consts::ERROR_COLOUR);
  EspGL::delay(2000);
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi

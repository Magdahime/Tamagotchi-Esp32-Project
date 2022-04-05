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

}  // namespace State
}  // namespace App
}  // namespace tamagotchi

#include "EndState.hpp"

#include "EspGLUtils.hpp"
#include "Globals.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void EndState::handleEvent(Event::Event Event) {}

void EndState::run() {
  init();
  mainLoop();
  deinit();
}

void EndState::init() {
  ESP_LOGI(TAG_, "Filling window black");
  Globals::game.screen().fill(EspGL::colours::BLACK);
  Globals::game.print("###########\nGAME OVER\n###########", {0, 0},
                      EspGL::colours::GREEN);
}

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
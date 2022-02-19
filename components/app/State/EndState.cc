#include "EndState.hpp"

#include "EspGLUtils.hpp"
#include "Globals.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void EndState::handleEvent(Event::Event Event) {}

void EndState::run() {
  ESP_LOGI(TAG_, "Filling window black");
  tamagotchi::App::Globals::game.screen().fill(EspGL::colours::BLACK);
  tamagotchi::App::Globals::game.print("###########\nGAME OVER\n###########",
                                       {0, 0}, EspGL::colours::GREEN);
}
}  // namespace State

}  // namespace App

}  // namespace tamagotchi
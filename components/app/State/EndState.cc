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

void EndState::mainLoop() {
  ESP_LOGI(TAG_, "Serializing pet");
  Serializer::Serializer serializer;
  serializer.serialize(&(Globals::game.pet()),
                       Globals::defaultValues::SERIALIZED_PET_PATH);
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
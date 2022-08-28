#include "EndState.hpp"

#include "EspGLUtils.hpp"
#include "Globals.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void EndState::handleEvent(Event::Event event) {}

void EndState::mainLoop() {
  ESP_LOGI(TAG_, "Serializing pet");
  Serializer::Serializer serializer;
  serializer.serialize(&(Globals::game.pet()),
                       Globals::defaultValues::SERIALIZED_PET_PATH);
}

void EndState::stateInit() {
  ESP_LOGI(TAG_, "Filling window black");
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "###########\nGAME OVER\n###########",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN);
}

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
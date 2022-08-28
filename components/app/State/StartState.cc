#include "StartState.hpp"

#include "DrawablePet.hpp"
#include "EspGLUtils.hpp"
#include "Event.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "State.hpp"
#include "StateUtils.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void StartState::handleEvent(Event::Event event) {}

void StartState::stateInit() {
  ESP_LOGI(TAG_, "Filling window black");
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "###########\nWelcome!\n###########",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN);
  EspGL::delay(2000);
  auto deserializePetFileHandle = Globals::spiffsDriver.getFileDescriptor(
      Globals::defaultValues::SERIALIZED_PET_PATH);
  if (deserializePetFileHandle.is_open()) {
    ESP_LOGI(TAG_, "Deserializing pet");
    Serializer::Serializer serializer;
    serializer.deserialize(deserializePetFileHandle, Globals::game.pet());
  } else {
    ESP_LOGI(TAG_, "Generating pet");
    App::Pet::PetGenerator<uint16_t> petGenerator(
        Globals::defaultValues::PET_COMPONENTS_PATH);
    auto bitmapPet = petGenerator.generate();
    Pet::DrawablePet<uint16_t> drawablePet(bitmapPet);
    drawablePet.setStart(EspGL::Vect2(0, Game::consts::SCREEN_HEIGHT / 2));
    Globals::game.setPet(drawablePet);
  }
  ESP_LOGI(TAG_, "Filling window black");
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "###########\nThis is your pet:\n###########",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN);
  Globals::game.pet().draw(Globals::game.screen());
  Globals::game.print(
      "Click to continue...",
      {{0, 280}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN, 1);
}
void StartState::mainLoop() {
  while (tamagotchi::App::Globals::game.eventQueue()
             .getQueue(consts::USER_INPUT_WAIT_TIME)
             .type_ == Event::EventTypes::gpio) {
    Globals::game.setNextState(StateType::MainMenu);
    break;
  }
}

void StartState::deinit() {}

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
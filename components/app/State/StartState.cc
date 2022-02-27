#include "StartState.hpp"

#include "EspGLUtils.hpp"
#include "Event.hpp"
#include "Globals.hpp"
#include "State.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void StartState::handleEvent(Event::Event Event) {}

void StartState::init() {
  ESP_LOGI(TAG_, "Filling window black");
  Globals::game.screen().fill(EspGL::colours::BLACK);
  Globals::game.print("###########\nWelcome!\n###########", {0, 0},
                      EspGL::colours::GREEN);
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
    auto pet = petGenerator.generate();
    Globals::game.setPet(pet);
  }
  ESP_LOGI(TAG_, "Filling window black");
  Globals::game.screen().fill(EspGL::colours::BLACK);
  Globals::game.print("###########\nThis is your pet:\n###########", {0, 0},
                      EspGL::colours::GREEN);
  Globals::game.pet().draw(Globals::game.screen(),
                           {0, Globals::game.screen().height() / 2});
  Globals::game.print("Click to continue...", {0, 280}, EspGL::colours::GREEN,
                      1);
}
void StartState::mainLoop() {
  constexpr int timeToWait = 100;
  while (tamagotchi::App::Globals::game.getQueue(timeToWait).type_ ==
         Event::EventTypes::gpio) {
    Globals::game.setNextState(StateType::End);
    break;
  }
}

void StartState::deinit() {}

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
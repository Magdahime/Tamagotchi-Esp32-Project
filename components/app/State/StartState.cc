#include "StartState.hpp"

#include "EspGLUtils.hpp"
#include "Globals.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void StartState::handleEvent(Event::Event Event) {}

void StartState::run() {
  ESP_LOGI(TAG_, "StartState running...");
  ESP_LOGI(TAG_, "Filling window black");
  tamagotchi::App::Globals::game.screen().fill(EspGL::colours::BLACK);
  tamagotchi::App::Globals::game.print("###########\nWelcome!\n###########",
                                       {0, 0}, EspGL::colours::GREEN);
  auto deserializePetFileHandle = Globals::spiffsDriver.getFileDescriptor(
      Globals::defaultValues::SERIALIZED_PET_PATH);
  if (deserializePetFileHandle.is_open()) {
    ESP_LOGI(TAG_, "Deserializing pet");
    Serializer::Serializer serializer;
    serializer.deserialize(deserializePetFileHandle,
                           tamagotchi::App::Globals::game.pet());
  } else {
    ESP_LOGI(TAG_, "Generating pet");
    App::Pet::PetGenerator<uint16_t> petGenerator(
        Globals::defaultValues::PET_COMPONENTS_PATH);
    auto pet = petGenerator.generate();
    tamagotchi::App::Globals::game.setPet(pet);
  }
  ESP_LOGI(TAG_, "Filling window black");
  tamagotchi::App::Globals::game.screen().fill(EspGL::colours::BLACK);
  tamagotchi::App::Globals::game.print(
      "###########\nThis is your pet:\n###########", {0, 0},
      EspGL::colours::GREEN);
  tamagotchi::App::Globals::game.pet().draw(
      tamagotchi::App::Globals::game.screen(),
      {tamagotchi::App::Globals::game.screen().width() / 2,
       tamagotchi::App::Globals::game.screen().height() / 2});
  tamagotchi::App::Globals::game.print(
      "Click to continue...", {0, 280},
      EspGL::colours::GREEN,1);
  ESP_LOGI(TAG_, "StartState exiting...");
  tamagotchi::App::Globals::game.shiftState(StateType::End);
}

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
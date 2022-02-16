#include "StartState.hpp"

#include "EspGLUtils.hpp"
#include "Globals.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void StartState::handleEvent(Event::Event Event) {}

void StartState::run() {
  ESP_LOGI(TAG_, "StartState running...");
  tamagotchi::App::Globals::game.screen().fill(EspGL::colours::BLACK);
  ESP_LOGI(TAG_, "Filling window black");
  EspGL::Text<uint16_t> welcomeText(
      "#######\nWelcome!\n#######", tamagotchi::App::Globals::game.font(),
      EspGL::Colour<uint16_t>(EspGL::colours::GREEN));
  welcomeText.draw(tamagotchi::App::Globals::game.screen(), {0, 0});

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

  ESP_LOGI(TAG_, "StartState exiting...");
}

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
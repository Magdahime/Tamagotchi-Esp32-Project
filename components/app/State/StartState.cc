#include "StartState.hpp"

namespace tamagotchi {

namespace App {

namespace State {

void StartState::handleEvent(Event::Event Event) {}

void StartState::run() {
  EspGL::Text<uint16_t> welcomeText(
      "Welcome!", tamagotchi::App::Game::Game::getInstance().font(),
      EspGL::Colour<uint16_t>(EspGL::colours::GREEN));
  welcomeText.draw(
      tamagotchi::App::Game::Game::getInstance().screen(),
      {tamagotchi::App::Game::Game::getInstance().screen().width(),
       tamagotchi::App::Game::Game::getInstance().screen().height()});

  auto deserializePetFileHandle = Globals::spiffsDriver.getFileDescriptor(
      Globals::defaultValues::SERIALIZED_PET_PATH);
  if (deserializePetFileHandle.is_open()) {
    Serializer::Serializer serializer;
    serializer.deserialize(deserializePetFileHandle,
                           tamagotchi::App::Game::Game::getInstance().pet());
  } else {
    App::Pet::PetGenerator<uint16_t> petGenerator(
        Globals::defaultValues::PET_COMPONENTS_PATH);
    auto pet = petGenerator.generate();
    tamagotchi::App::Game::Game::getInstance().setPet(pet);
  }
}

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
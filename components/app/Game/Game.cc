#include "Game.hpp"

#include <stdint.h>

#include <memory>

#include "EndState.hpp"
#include "Event.hpp"
#include "Globals.hpp"
#include "Gomoku/EndGameState.hpp"
#include "Gomoku/HostDutiesState.hpp"
#include "Gomoku/MiniGameState.hpp"
#include "Gomoku/PlayerTurnState.hpp"
#include "Gomoku/WaitingForTurnState.hpp"
#include "MainMenuState.hpp"
#include "SPIFFSDriver.hpp"
#include "StartState.hpp"

namespace tamagotchi {
namespace App {
namespace Game {

Game::Game()
    : eventQueue_(EVENT_QUEUE_SIZE),
      gomokuBoard_(EspGL::Vect2(0, screen_.width()),
                   EspGL::Vect2(0, screen_.height())) {
  initializeScreen();
  EspGL::FontLoader fLoader(Globals::spiffsDriver.getFileDescriptor(
      Globals::defaultValues::FONT_FILE_PATH));
  ESP_LOGI(TAG_, "Loading font");
  font_ = fLoader.load();
  createStates();
}

void Game::run() {
  currentState_ = State::StateType::Start;
  nextState_ = State::StateType::Start;
  shiftState();
  ESP_LOGI(TAG_, "End of the Game.");
}

void Game::createStates() {
  ESP_LOGI(TAG_, "Creating states");
  states_.emplace(State::StateType::Start,
                  std::make_unique<State::StartState>());
  states_.emplace(State::StateType::MainMenu,
                  std::make_unique<State::MainMenuState>());
  states_.emplace(State::StateType::MiniGame,
                  std::make_unique<State::MiniGameState>());
  states_.emplace(State::StateType::GameHostDuties,
                  std::make_unique<State::HostDutiesState>());
  states_.emplace(State::StateType::PlayerTurn,
                  std::make_unique<State::PlayerTurnState>());
  states_.emplace(State::StateType::WaitingForTurn,
                  std::make_unique<State::WaitingForTurnState>());
  states_.emplace(State::StateType::EndMiniGame,
                  std::make_unique<State::EndGameState>());

  states_.emplace(State::StateType::End, std::make_unique<State::EndState>());
}

void Game::initializeScreen() {
  ESP_LOGI(TAG_, "Initializing screen");
  ST7789::structs::gpio_config_t gpio = {
      .gpioMosi = (gpio_num_t)ST7789::consts::GPIOMOSI,
      .gpioSclk = (gpio_num_t)ST7789::consts::GPIOSCLK,
      .gpioCs = (gpio_num_t)ST7789::consts::GPIOCS,
      .gpioDc = (gpio_num_t)ST7789::consts::GPIODC,
      .gpioReset = (gpio_num_t)ST7789::consts::GPIORESET,
      .gpioBl = (gpio_num_t)ST7789::consts::GPIOBL};

  ST7789::structs::lcd_config_t lcd{.width = ST7789::consts::WIDTH,
                                    .height = ST7789::consts::HEIGHT,
                                    .offsetx = ST7789::consts::OFFSETX,
                                    .offsety = ST7789::consts::OFFSETY};

  ST7789::structs::st7789_config_t config = {.gpio = gpio, .lcd = lcd};

  screen_ =
      EspGL::Screen<uint16_t>(ST7789::consts::WIDTH, ST7789::consts::HEIGHT,
                              std::make_unique<ST7789::ST7789VWDriver>(config));
}

void Game::shiftState() {
  auto nextState = states_[nextState_].get();
  ESP_LOGI(TAG_, "Running new state: %s", nextState->toString().c_str());
  currentState_ = nextState_;
  nextState->run();
}

void Game::print(std::string message, EspGL::Hitbox position,
                 EspGL::Colour<uint16_t> colour, int characterSize) {
  ESP_LOGI(TAG_, "Writing text on screen: %s", message.c_str());
  EspGL::Text<uint16_t> text(message, position, font_, colour, characterSize);
  text.draw(screen_);
  tamagotchi::EspGL::delay(10);
}

GomokuNetworking::structs::PetParams Game::getPetParams() {
  return GomokuNetworking::structs::PetParams{
      pet_.body().first, pet_.eyes().first, pet_.face().first};
}

}  // namespace Game
}  // namespace App
}  // namespace tamagotchi
#include "Game.hpp"

#include <stdint.h>

#include <memory>

#include "SPIFFSDriver.hpp"

namespace tamagotchi {
namespace App {
namespace Game {

xQueueHandle Game::eventQueue_ =
    xQueueCreate(EVENT_QUEUE_SIZE, sizeof(Event::Event));

Game::Game() {
  initializeScreen();
  createStates();
  currentState_ = states_[State::StateType::Start].get();
}

void Game::createStates() {
  states_.emplace(State::StateType::Start,
                  std::make_unique<State::StartState>());
}

void Game::initializeScreen() {
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

void Game::mainLoop() {
  // while (xQueueReceive(eventQueue_, &(pxRxedPointer), (TickType_t)10) ==
  //        pdPASS) {
  // }
}

}  // namespace Game
}  // namespace App
}  // namespace tamagotchi
#pragma once
#include <stdint.h>

#include <algorithm>
#include <map>
#include <memory>

#include "EspGLDriver.hpp"
#include "EspGLScreen.hpp"
#include "EspGLUtils.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "Graphics/Text/EspGLText.hpp"
#include "Pet.hpp"
#include "SPIFFSDriver.hpp"
#include "ST7789Driver.hpp"
#include "State.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

namespace tamagotchi {
namespace App {
namespace Game {

constexpr int EVENT_QUEUE_SIZE = 10;

class Game {
 public:
  Game();
  ~Game() = default;
  void run();
  void mainLoop();
  void shiftState(const State::StateType& newState);

  static bool putQueue(Event::Event* event);

  Pet::Pet<uint16_t>& pet() { return pet_; }
  void setPet(Pet::Pet<uint16_t>& pet) { pet_ = pet; }

  void setFont(EspGL::Font& font) { font_ = font; }
  EspGL::Font& font() { return font_; }

  EspGL::Screen<uint16_t>& screen() { return screen_; }
  void print(std::string message, EspGL::Point position,
             EspGL::Colour<uint16_t> colour, int characterSize = 3);

 private:
  State::State* currentState_;
  Pet::Pet<uint16_t> pet_;
  std::map<State::StateType, std::unique_ptr<State::State>> states_;
  EspGL::Screen<uint16_t> screen_;
  static xQueueHandle eventQueue_;
  static constexpr char TAG_[] = "Game";
  EspGL::Font font_;

  void initializeScreen();
  void createStates();
};

}  // namespace Game
}  // namespace App
}  // namespace tamagotchi
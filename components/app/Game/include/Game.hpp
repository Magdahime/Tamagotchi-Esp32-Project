#pragma once
#include <stdint.h>

#include <algorithm>
#include <map>
#include <memory>

#include "EspGLDriver.hpp"
#include "EspGLScreen.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "Graphics/Text/EspGLText.hpp"
#include "Pet.hpp"
#include "SPIFFSDriver.hpp"
#include "ST7789Driver.hpp"
#include "StartState.hpp"
#include "State.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

namespace tamagotchi {
namespace App {
namespace Game {

constexpr int EVENT_QUEUE_SIZE = 10;

class Game {
 public:
  Game(Game const&) = delete;
  void operator=(Game const&) = delete;
  static Game& getInstance() {
    static Game instance;
    return instance;
  }

  ~Game() = default;
  void mainLoop();
  void shiftState(const State::StateType& newState);
  static bool putQueue(Event::Event* event);

  Pet::Pet<uint16_t>& pet() { return pet_; }
  void setPet(Pet::Pet<uint16_t>& pet) { pet_ = pet; }

  void setFont(EspGL::Font& font) { font_ = font; }
  EspGL::Font& font() { return font_; }

  EspGL::Screen<uint16_t>& screen() { return screen_; }

 private:
  State::State* currentState_;
  Pet::Pet<uint16_t> pet_;
  std::map<State::StateType, std::unique_ptr<State::State>> states_;
  EspGL::Screen<uint16_t> screen_;
  static xQueueHandle eventQueue_;
  EspGL::Font font_;

  void initializeScreen();
  void createStates();
  Game();
};

}  // namespace Game
}  // namespace App
}  // namespace tamagotchi
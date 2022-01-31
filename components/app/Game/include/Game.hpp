#pragma once
#include <stdint.h>

#include <algorithm>
#include <map>
#include <memory>

#include "EspGLDriver.hpp"
#include "EspGLScreen.hpp"
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
  void mainLoop();
  void shiftState(const State::StateType& newState);
  static bool putQueue(Event::Event* event);
  ~Game() = default;
  EspGL::Screen<uint16_t>& screen() { return screen_; }

 private:
  State::State* currentState_;
  std::map<State::StateType, std::unique_ptr<State::State>> states_;
  EspGL::Screen<uint16_t> screen_;
  static xQueueHandle eventQueue_;

  void initializeScreen();
  void createStates();
  Game();
};

}  // namespace Game
}  // namespace App
}  // namespace tamagotchi
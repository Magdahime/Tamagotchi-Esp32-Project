#pragma once
#include <stdint.h>

#include <algorithm>
#include <map>
#include <memory>

#include "DrawablePet.hpp"
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
constexpr int PET_SCALE = 5;

class Game {
 public:
  Game();
  ~Game() = default;
  void run();
  void shiftState();

  static bool putQueue(Event::Event event);
  static void putQueueFromISR(Event::Event event);
  static void clearQueue();
  static Event::Event getQueue(int ms);

  Pet::DrawablePet<uint16_t>& pet() { return pet_; }
  void setPet(Pet::DrawablePet<uint16_t>& pet) { pet_ = pet; }

  void setFont(EspGL::Font& font) { font_ = font; }
  EspGL::Font& font() { return font_; }

  EspGL::Screen<uint16_t>& screen() { return screen_; }

  void print(std::string message,
             EspGL::Hitbox position,
             EspGL::Colour<uint16_t> colour, int characterSize = 3);

  State::StateType currentState() { return currentState_; }
  State::StateType nextState() { return nextState_; }

  void setNextState(State::StateType newState) { nextState_ = newState; }

 private:
  State::StateType currentState_;
  State::StateType nextState_;

  Pet::DrawablePet<uint16_t> pet_;
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
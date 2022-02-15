#pragma once
#include "Event.hpp"
#include "esp_log.h"

namespace tamagotchi {

namespace App {

namespace State {

class State {
 public:
  virtual ~State() {}
  virtual void handleEvent(Event::Event Event) = 0;
  virtual void run() = 0;

 private:
};

enum class StateType { Start, MainMenu, MiniGame, End };

}  // namespace State

}  // namespace App

}  // namespace tamagotchi

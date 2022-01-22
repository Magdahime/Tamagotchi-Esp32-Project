#pragma once
#include "Event.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class State {
 public:
  virtual void handleEvent(Event::Event Event) = 0;

 private:
};

enum class StateType { Start, MainMenu, MiniGame, End };

}  // namespace State

}  // namespace App

}  // namespace tamagotchi

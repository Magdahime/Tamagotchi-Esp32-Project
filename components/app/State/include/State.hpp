#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Drawable.hpp"
#include "Event.hpp"
#include "esp_log.h"

namespace tamagotchi {

namespace App {

namespace State {

enum class StateType {
  Start,
  MainMenu,
  MiniGame,
  GameHost,
  NormalPlayer,
  GomokuMakeMove,
  End
};

class State {
 public:
  virtual ~State() {}
  virtual void handleEvent(Event::Event event) = 0;
  virtual void init() = 0;
  virtual void mainLoop() = 0;
  virtual std::string toString() = 0;
  virtual void deinit() = 0;
  virtual void run();
  void loop();

 private:
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi

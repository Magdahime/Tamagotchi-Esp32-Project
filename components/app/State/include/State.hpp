#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Event.hpp"
#include "esp_log.h"

namespace tamagotchi {

namespace App {

namespace State {

enum class StateType {
  Start,
  MainMenu,
  MiniGame,
  GameHostDuties,
  PlayerTurn,
  WaitingForTurn,
  EndMiniGame,
  End,
};

class State {
 public:
  virtual ~State() {}
  virtual void handleEvent(Event::Event event) = 0;
  virtual void init() final;
  virtual void stateInit() = 0;
  virtual void mainLoop() = 0;
  virtual std::string toString() = 0;
  virtual void deinit() = 0;
  void loop();

  void displayErrorMessage(std::string message, EspGL::Vect2 placing);

 private:
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi

#pragma once
#include "Event.hpp"
#include "State.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class MainMenuState : public State {
 public:
  virtual void handleEvent(Event::Event Event) override;
  virtual void run() override;

 private:
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
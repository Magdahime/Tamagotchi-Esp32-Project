#pragma once
#include "State.hpp"
#include "Event.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class StartState : public State {
 public:
  StartState() = default;
  virtual void handleEvent(Event::Event Event) override;
  virtual void run() override;

 private:
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
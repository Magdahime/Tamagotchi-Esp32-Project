#pragma once
#include "Event.hpp"
#include "Game.hpp"
#include "State.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class EndState : public State {
 public:
  virtual void handleEvent(Event::Event Event) override;
  virtual void run() override;

 private:
  static constexpr char TAG_[] = "EndState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
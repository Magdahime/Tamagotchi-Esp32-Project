#pragma once
#include "State.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class GomokuState : public State {
 public:
  virtual void handleEvent(Event::Event Event) override;

 private:
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
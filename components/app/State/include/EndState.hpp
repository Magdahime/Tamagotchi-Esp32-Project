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
  virtual void init() override;
  virtual void mainLoop() override {}
  virtual void deinit() override {}
  void run() override;
  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "EndState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
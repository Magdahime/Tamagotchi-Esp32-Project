#pragma once
#include "Event.hpp"
#include "Game.hpp"
#include "Serializer.hpp"
#include "State.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class EndState : public State {
 public:
  EndState() = default;
  virtual ~EndState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void stateInit() override;
  virtual void mainLoop() override;
  virtual void deinit() override {}
  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "EndState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
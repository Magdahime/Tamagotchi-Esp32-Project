#pragma once
#include "Event.hpp"
#include "State.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class MiniGameState : public State {
 public:
  MiniGameState();
  virtual ~MiniGameState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;
  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "MiniGameState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
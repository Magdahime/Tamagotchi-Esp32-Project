#pragma once
#include "State.hpp"

namespace tamagotchi {
namespace App {
namespace State {

class GameHostState : public State {
 public:
  GameHostState() = default;
  virtual ~GameHostState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "GameHostState";
};

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
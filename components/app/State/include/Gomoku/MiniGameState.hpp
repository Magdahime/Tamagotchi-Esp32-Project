#pragma once
#include <stdint.h>

#include "Event.hpp"
#include "GomokuDrawable.hpp"
#include "GomokuNetworking.hpp"
#include "State.hpp"
#include "StateUtils.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class MiniGameState : public State {
 public:
  MiniGameState();
  virtual ~MiniGameState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void stateInit() override;
  virtual void mainLoop() override;
  virtual void deinit() override;
  virtual std::string toString() override { return TAG_; }

 private:
  void startNetworkingTask();
  static constexpr char TAG_[] = "MiniGameState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
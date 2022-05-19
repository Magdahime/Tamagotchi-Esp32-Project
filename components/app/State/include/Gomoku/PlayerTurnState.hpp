#pragma once
#include "State.hpp"
#include "StateUtils.hpp"
namespace tamagotchi::App::State {

class PlayerTurnState : public State {
 public:
  PlayerTurnState() = default;
  virtual ~PlayerTurnState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "PlayerTurnState";
};

}  // namespace tamagotchi::App::State
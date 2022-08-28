#pragma once
#include "State.hpp"
#include "StateUtils.hpp"
namespace tamagotchi::App::State {

class EndGameState : public State {
 public:
  EndGameState() = default;
  virtual ~EndGameState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void stateInit() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "EndGameState";
};

}  // namespace tamagotchi::App::State
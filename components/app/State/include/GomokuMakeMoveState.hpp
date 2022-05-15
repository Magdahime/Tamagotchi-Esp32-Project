#pragma once
#include "GomokuDrawable.hpp"
#include "State.hpp"
#include "StateUtils.hpp"
namespace tamagotchi::App::State {

class GomokuMakeMoveState : public State {
 public:
  GomokuMakeMoveState() = default;
  virtual ~GomokuMakeMoveState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "GomokuMakeMoveState";
};

}  // namespace tamagotchi::App::State
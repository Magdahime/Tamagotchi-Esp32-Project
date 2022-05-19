#pragma once
#include "GomokuNetworkingConf.hpp"
#include "State.hpp"
#include "StateUtils.hpp"
namespace tamagotchi::App::State {

class WaitingForTurnState : public State {
 public:
  WaitingForTurnState() = default;
  virtual ~WaitingForTurnState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  void updateBoard(Gomoku::structs::GomokuMoveUpdateFromPlayer* nextMove);
  static constexpr char TAG_[] = "WaitingForTurnState";
};

}  // namespace tamagotchi::App::State
#pragma once
#include <stdint.h>

#include "GomokuNetworkingConf.hpp"
#include "State.hpp"
#include "StateUtils.hpp"
namespace tamagotchi::App::State {

constexpr int MAX_WAITING_DOTS = 6;

class WaitingForTurnState : public State {
 public:
  WaitingForTurnState() : counter_(0) {}
  virtual ~WaitingForTurnState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  bool myTurn_;
  int8_t counter_;
  void sendAck(uint32_t magic);
  void updateGomokuPlayersColourInfo(
      GomokuNetworking::gomoku_payload_array_t dataArray);
  void displayWaitingMessage();
  void updateBoard(
      GomokuNetworking::structs::GomokuMoveUpdateFromPlayer* nextMove);
  static constexpr char TAG_[] = "WaitingForTurnState";
};

}  // namespace tamagotchi::App::State
#pragma once
#include <stdint.h>

#include "GomokuNetworking.hpp"
#include "State.hpp"

namespace tamagotchi {
namespace App {
namespace State {

class GameHostState : public State {
 public:
  GameHostState() : macAddresses_(Gomoku::GomokuNetworking::playersMacs()) {}
  virtual ~GameHostState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

  void makeMove();
  void sendNotificationAboutNextPlayer(Gomoku::mac_address_t player);
  void waitForPlayerMove(Gomoku::mac_address_t player);
  Gomoku::structs::GomokuMoveUpdateFromPlayer sendMoveUpdate();
  bool updateBoard(Gomoku::structs::GomokuMoveUpdateFromPlayer nextMove);

 private:
  static constexpr char TAG_[] = "GameHostState";
  std::vector<Gomoku::mac_address_t>& macAddresses_;

  void sendAll(Gomoku::structs::GomokuData& sendData);
};

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
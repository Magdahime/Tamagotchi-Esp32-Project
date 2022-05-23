#pragma once
#include <stdint.h>

#include "GomokuNetworking.hpp"
#include "State.hpp"

namespace tamagotchi {
namespace App {
namespace State {
// After this amount of time send next notification to current player
constexpr int NOTIFICATION_PERIOD_SEC = 10;
// After this amount of time we assume this player is dead and we remove it from
// known players
constexpr int NOTIFICATION_TIMEOUT_SEC = 60;

class HostDutiesState : public State {
 public:
  HostDutiesState()
      : macAddresses_(Gomoku::GomokuNetworking::playersMacs()),
        currentPlayer_(macAddresses_.begin()),
        timestamp_(0) {}
  virtual ~HostDutiesState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

  void sendNotificationAboutDeadPlayer();
  void sendNotificationToCurrentPlayer();
  void sendMoveUpdate(
      std::array<uint8_t, Gomoku::consts::ESPNOW_PAYLOAD_MAX>& payload);
  bool updateBoard(Gomoku::structs::GomokuMoveUpdateFromPlayer* nextMove);

  void sendEndOfGameMessage();

 private:
  static constexpr char TAG_[] = "HostDutiesState";
  std::vector<Gomoku::mac_address_t> macAddresses_;
  std::vector<Gomoku::mac_address_t>::iterator currentPlayer_;
  int64_t timestamp_;

  void removePlayerFromList(Gomoku::mac_address_t toRemove);
  void sendAll(Gomoku::structs::GomokuData& sendData);
};

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
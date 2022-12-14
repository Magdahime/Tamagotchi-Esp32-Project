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
  HostDutiesState() : macAddresses_(), currentPlayer_(macAddresses_.begin()) {}
  virtual ~HostDutiesState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void stateInit() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

  void sendNotificationAboutDeadPlayer(GomokuNetworking::mac_address_t deadMac);
  void sendNotificationAboutCurrentPlayer();
  void sendMoveUpdate(
      std::array<uint8_t, GomokuNetworking::consts::ESPNOW_PAYLOAD_MAX>&
          payload);
  bool updateBoard(
      GomokuNetworking::structs::GomokuMoveUpdateFromPlayer* nextMove);
  void sendEndOfGameMessage();
  static void setInit() { init_ = false; }

 private:
  static bool init_;
  void updateColourConfig(
      std::array<GomokuNetworking::structs::Colour2Player,
                 GomokuNetworking::consts::MAX_GOMOKU_PLAYERS>& colour2Player);
  void sendColourConfig();
  void displayOrderMessage(GomokuNetworking::mac_address_t nextPlayer);
  void handleHostMove();

  static constexpr char TAG_[] = "HostDutiesState";
  std::vector<GomokuNetworking::mac_address_t> macAddresses_;
  std::vector<GomokuNetworking::mac_address_t>::iterator currentPlayer_;

  void removePlayerFromList(GomokuNetworking::mac_address_t toRemove);
  void sendAll(GomokuNetworking::structs::GomokuData sendData);
};

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
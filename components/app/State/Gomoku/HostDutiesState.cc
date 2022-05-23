#include "Gomoku/HostDutiesState.hpp"

#include <stdint.h>

#include <algorithm>

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworkingConf.hpp"

using namespace tamagotchi::App::Gomoku;

namespace tamagotchi {
namespace App {
namespace State {

void HostDutiesState::handleEvent(Event::Event event) {}
void HostDutiesState::init() { currentPlayer_ = macAddresses_.begin(); }

void HostDutiesState::mainLoop() {
  structs::GomokuEvent msg;

  // Waiting too long for answer
  auto diff = (xTaskGetTickCount() / configTICK_RATE_HZ) - timestamp_;
  if (diff > NOTIFICATION_PERIOD_SEC) {
    sendNotificationAboutDeadPlayer();
    removePlayerFromList(*currentPlayer_);
    currentPlayer_++;
  } else if (diff > NOTIFICATION_TIMEOUT_SEC) {
    sendNotificationToCurrentPlayer();
  }

  // WE RECEIVED INFORMATION FROM CURRENT PLAYER
  if (GomokuNetworking::receiveQueue().getQueue(msg) == pdPASS &&
      msg.macAddress == *(currentPlayer_)) {
    auto payload = Gomoku::GomokuNetworking::unpackData(msg);
    if (payload.empty()) return;
    sendMoveUpdate(payload);
    auto result = updateBoard(
        reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(payload.data()));
    if (result) {
      sendEndOfGameMessage();
      Globals::game.setNextState(StateType::EndMiniGame);
    }
    currentPlayer_++;
    sendNotificationToCurrentPlayer();
  }
  if (currentPlayer_ == macAddresses_.end()) {
    Globals::game.setNextState(StateType::PlayerTurn);
  }
}

void HostDutiesState::sendNotificationToCurrentPlayer() {
  ESP_LOGI(TAG_, "Sending notification to current player.");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::SENDING_ORDER,
                               0,
                               0,
                               {}};
  memcpy(sendData.payload.data(), (*currentPlayer_).data(), ESP_NOW_ETH_ALEN);
  structs::GomokuDataWithRecipient finalMessage{*(currentPlayer_), sendData};
  GomokuNetworking::sendingQueue().putQueue(finalMessage);
  timestamp_ = xTaskGetTickCount() / configTICK_RATE_HZ;
}

void HostDutiesState::sendMoveUpdate(gomoku_payload_array_t& payload) {
  ESP_LOGI(TAG_, "Sending update about next move.");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::SENDING_MOVE,
                               0,
                               0,
                               {}};
  memcpy(sendData.payload.data(), payload.data(),
         sizeof(structs::GomokuMoveUpdateFromPlayer));
  sendAll(sendData);
}

void HostDutiesState::sendAll(structs::GomokuData& sendData) {
  ESP_LOGI(TAG_, "Sending message to all known players.");
  sendData.crc =
      esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const*>(&sendData),
                   consts::ESPNOW_SEND_LEN);
  for (const auto& mac : macAddresses_) {
    structs::GomokuDataWithRecipient finalMessage{mac, sendData};
    GomokuNetworking::sendingQueue().putQueue(finalMessage);
  }
}

bool HostDutiesState::updateBoard(
    structs::GomokuMoveUpdateFromPlayer* nextMove) {
  ESP_LOGI(TAG_, "Updating board.");
  Globals::game.gomokuBoard().markMove(
      Globals::game.gomokuBoard().player2Int(nextMove->player), nextMove->move);
  return Globals::game.gomokuBoard().isWinner();
}

void HostDutiesState::deinit() {}

void HostDutiesState::sendEndOfGameMessage() {
  ESP_LOGI(TAG_, "Sending end of game message");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::END_OF_GAME,
                               0,
                               0,
                               {}};
  sendAll(sendData);
}

void HostDutiesState::sendNotificationAboutDeadPlayer() {
  ESP_LOGI(TAG_, "Sending message about dead player");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::DEAD_PLAYER,
                               0,
                               0,
                               {}};
  memcpy(sendData.payload.data(), currentPlayer_->data(),
         sizeof(currentPlayer_));
  sendAll(sendData);
}

void HostDutiesState::removePlayerFromList(mac_address_t toRemove) {
  macAddresses_.erase(
      std::remove(macAddresses_.begin(), macAddresses_.end(), toRemove),
      macAddresses_.end());
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
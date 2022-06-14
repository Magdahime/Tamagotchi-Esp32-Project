#include "Gomoku/HostDutiesState.hpp"

#include <stdint.h>

#include <algorithm>

#include "ColourProvider.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworkingConf.hpp"

using namespace tamagotchi::App::GomokuNetworking;

namespace tamagotchi {
namespace App {
namespace State {

void HostDutiesState::handleEvent(Event::Event event) {}
void HostDutiesState::init() {
  currentPlayer_ = macAddresses_.begin();
  sendColourConfig();
}

void HostDutiesState::mainLoop() {
  auto hostParams = GomokuNetworking::GomokuNetworking::hostParams();
  if (hostParams.newMove && hostParams.acksCollected) {
    structs::GomokuEvent msg;
    GomokuNetworking::GomokuNetworking::receiveQueue().getQueue(msg);
    if (msg.macAddress == *(currentPlayer_)) {
      auto gomokuData = GomokuNetworking::GomokuNetworking::unpackData(msg);
      sendMoveUpdate(gomokuData.payload);
      auto result =
          updateBoard(reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(
              gomokuData.payload.data()));
      if (result) {
        sendEndOfGameMessage();
        Globals::game.setNextState(StateType::EndMiniGame);
      }
      currentPlayer_++;
      sendNotificationToCurrentPlayer();
    }
    hostParams.newMove = false;
  }

  while (!hostParams.disconnectedPlayers.empty()) {
    if (macAddresses_.empty()) {
      Globals::game.setNextState(StateType::EndMiniGame);
    }
    auto disconnectedMac = hostParams.disconnectedPlayers.getQueue();
    sendNotificationAboutDeadPlayer(disconnectedMac);
    if (disconnectedMac == *currentPlayer_) {
      currentPlayer_++;
      sendNotificationToCurrentPlayer();
    }
    macAddresses_.erase(std::remove(macAddresses_.begin(), macAddresses_.end(),
                                    disconnectedMac),
                        macAddresses_.end());
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
  GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
  timestamp_ = xTaskGetTickCount() / configTICK_RATE_HZ;
}

void HostDutiesState::sendMoveUpdate(gomoku_payload_array_t& payload) {
  ESP_LOGI(TAG_, "Sending update about next move.");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::SENDING_MOVE_TO_PLAYERS,
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
    GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
  }
}

bool HostDutiesState::updateBoard(
    structs::GomokuMoveUpdateFromPlayer* nextMove) {
  ESP_LOGI(TAG_, "Updating board.");
  Globals::game.gomokuBoard().markMove(nextMove->player, nextMove->move);
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

void HostDutiesState::sendNotificationAboutDeadPlayer(mac_address_t deadMac) {
  ESP_LOGI(TAG_, "Sending message about dead player");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::DEAD_PLAYER,
                               0,
                               0,
                               {}};
  memcpy(sendData.payload.data(), deadMac.data(), sizeof(deadMac));
  sendAll(sendData);
}

void HostDutiesState::removePlayerFromList(mac_address_t toRemove) {
  macAddresses_.erase(
      std::remove(macAddresses_.begin(), macAddresses_.end(), toRemove),
      macAddresses_.end());
}

void HostDutiesState::sendColourConfig() {
  ESP_LOGI(TAG_, "Sending config about colours.");
  auto colours = ColourProvider::getPossibleGomokuColours<uint16_t>();
  if (GomokuNetworking::consts::MAX_GOMOKU_PLAYERS - 1 > colours.size()) {
    throw std::runtime_error(
        "Too little colours o assign one to each player! :(");
  }

  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::SENDING_COLOUR_CONFIG,
                               0,
                               0,
                               {}};

  std::array<structs::Colour2Player, consts::MAX_GOMOKU_PLAYERS> colour2Player;
  for (auto i = 0; i < macAddresses_.size(); i++) {
    colour2Player[i] =
        structs::Colour2Player{colours[i].value(), macAddresses_[i]};
  }
  structs::ColourConfig config{colour2Player};
  memcpy(sendData.payload.data(), config.colour2Player.data(), sizeof(config));
  sendAll(sendData);
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
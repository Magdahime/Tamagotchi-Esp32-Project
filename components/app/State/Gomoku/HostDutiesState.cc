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

bool HostDutiesState::init_ = false;

void HostDutiesState::handleEvent(Event::Event event) {}
void HostDutiesState::stateInit() {
  ESP_LOGI(TAG_, "Start HostDuties!");
  macAddresses_ = GomokuNetworking::GomokuNetworking::playersMacs();
  if (!init_) {
    sendColourConfig();
    init_ = true;
  }
  currentPlayer_ = macAddresses_.begin();
  sendNotificationAboutCurrentPlayer();
  displayOrderMessage(*currentPlayer_);
}

void HostDutiesState::mainLoop() {
  auto hostParams = GomokuNetworking::GomokuNetworking::hostParams();
  if (hostParams.newMove && hostParams.acksCollected) {
    ESP_LOGI(TAG_, "There is new move!");
    structs::GomokuEvent msg;
    if ((GomokuNetworking::GomokuNetworking::hostQueue().getQueue(msg, 1000) ==
         pdPASS) &&
        msg.macAddress == *(currentPlayer_)) {
      auto gomokuData = GomokuNetworking::GomokuNetworking::unpackData(msg);
      sendAck(*currentPlayer_, gomokuData.magic);
      sendMoveUpdate(gomokuData.payload);
      auto result =
          updateBoard(reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(
              gomokuData.payload.data()));
      if (result || Globals::game.gomokuBoard().full()) {
        sendEndOfGameMessage();
        Globals::game.setNextState(StateType::EndMiniGame);
        return;
      }
      currentPlayer_++;
      hostParams.newMove = false;
      if (currentPlayer_ != macAddresses_.end()) {
        sendNotificationAboutCurrentPlayer();
        displayOrderMessage(*currentPlayer_);
      }
    }
    hostParams.newMove = false;
  }

  while (!hostParams.disconnectedPlayers.empty()) {
    ESP_LOGI(
        TAG_,
        "There are some players to be removed, because they disconnected.");
    auto disconnectedMac = hostParams.disconnectedPlayers.getQueue();
    sendNotificationAboutDeadPlayer(disconnectedMac);
    if (disconnectedMac == *currentPlayer_) {
      currentPlayer_++;
      sendNotificationAboutCurrentPlayer();
    }
    macAddresses_.erase(std::remove(macAddresses_.begin(), macAddresses_.end(),
                                    disconnectedMac),
                        macAddresses_.end());
  }

  if (macAddresses_.empty()) {
    Globals::game.setNextState(StateType::EndMiniGame);
    return;
  }

  if (currentPlayer_ == macAddresses_.end() &&
      GomokuNetworking::GomokuNetworking::hostQueue().elementsCount() == 0) {
    ESP_LOGI(TAG_, "Host turn to make a move.");
    Globals::game.setNextState(StateType::PlayerTurn);
    return;
  }
  EspGL::delay(GomokuNetworking::consts::ESPNOW_SEND_DELAY);
}

void HostDutiesState::sendNotificationAboutCurrentPlayer() {
  ESP_LOGI(TAG_, "Sending notification to current player.");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::SENDING_ORDER,
                               0,
                               esp_random(),
                               {}};
  memcpy(sendData.payload.data(), (*currentPlayer_).data(), ESP_NOW_ETH_ALEN);
  sendAll(sendData);
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

void HostDutiesState::sendAll(structs::GomokuData sendData) {
  ESP_LOGI(TAG_, "Sending message to all known players.");
  for (auto mac : macAddresses_) {
    sendData.magic = esp_random();
    sendData.crc =
        esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const*>(&sendData),
                     consts::ESPNOW_SEND_LEN);
    structs::GomokuDataWithRecipient finalMessage{mac, sendData};
    GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
  }
  ESP_LOGI(TAG_, "ELEMS: %d",
           GomokuNetworking::GomokuNetworking::sendingQueue().elementsCount());
}

bool HostDutiesState::updateBoard(
    structs::GomokuMoveUpdateFromPlayer* nextMove) {
  ESP_LOGI(TAG_, "Updating board.");
  ESP_LOGI(TAG_, "nextMove->player: " MACSTR, MAC2STR(nextMove->player));
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
  colour2Player[colour2Player.size() - 1] =
      structs::Colour2Player{colours[colour2Player.size() - 1].value(),
                             GomokuNetworking::GomokuNetworking::hostAddress()};

  structs::ColourConfig config{colour2Player};
  memcpy(sendData.payload.data(), config.colour2Player.data(), sizeof(config));
  sendAll(sendData);
  updateColourConfig(colour2Player);
}

void HostDutiesState::updateColourConfig(
    std::array<structs::Colour2Player, consts::MAX_GOMOKU_PLAYERS>&
        colour2Player) {
  ESP_LOGI(TAG_, "Update colours in host node.");
  auto& params = GomokuNetworking::GomokuNetworking::playersParams();
  auto& player2Colour = Globals::game.gomokuBoard().player2Colour();
  for (auto& elem : colour2Player) {
    player2Colour.emplace(std::make_pair(elem.player, elem.colourValue));
    auto it = std::find_if(params.begin(), params.end(), [&](auto pair) {
      return pair.first == elem.player;
    });
    (*it).second.setColour(EspGL::Colour16(elem.colourValue));
  }
}

void HostDutiesState::displayOrderMessage(mac_address_t nextPlayer) {
  ESP_LOGI(TAG_, "Display Order Message");
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "It is\nnext \nplayer turn!",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN);
  auto& params = GomokuNetworking::GomokuNetworking::playersParams();
  auto it = std::find_if(params.begin(), params.end(),
                         [&](auto pair) { return pair.first == nextPlayer; });
  (*it).second.setStart(EspGL::Vect2(0, Game::consts::SCREEN_HEIGHT / 2));
  (*it).second.draw(Globals::game.screen());
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
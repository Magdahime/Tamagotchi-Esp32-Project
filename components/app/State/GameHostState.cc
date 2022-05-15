#include "GameHostState.hpp"

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworkingConf.hpp"

namespace tamagotchi {
namespace App {
namespace State {

void GameHostState::handleEvent(Event::Event event) {}
void GameHostState::init() {}
void GameHostState::mainLoop() {
  while (true) {
    makeMove();
    sendMoveUpdate();
    for (const auto& player : macAddresses_) {
      sendNotificationAboutNextPlayer(player);
      waitForPlayerMove(player);
      auto move = sendMoveUpdate();
      if (updateBoard(move) == true) break;
    }
  }
}

void GameHostState::makeMove() {
  Globals::game.setNextState(StateType::GomokuMakeMove);
}

void GameHostState::sendNotificationAboutNextPlayer(
    Gomoku::mac_address_t player) {
  Gomoku::structs::GomokuData sendData{
      Gomoku::structs::GomokuCommunicationType::UNICAST,
      Gomoku::GomokuMessageStates::SENDING_ORDER,
      0,
      0,
      0,
      {}};
  memcpy(sendData.payload.data(), player.data(), ESP_NOW_ETH_ALEN);
  sendAll(sendData);
  xTaskNotifyGive(Gomoku::GomokuNetworking::gomokuNetworkingTask());
}

void GameHostState::waitForPlayerMove(Gomoku::mac_address_t player) {
  Gomoku::structs::GomokuEvent waitEvent{
      player, Gomoku::structs::GomokuWaitForAnswerFromHost{}};

  Gomoku::GomokuNetworking::gomokuQueue().putQueue(waitEvent);
  xTaskNotifyGive(Gomoku::GomokuNetworking::gomokuNetworkingTask());
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

Gomoku::structs::GomokuMoveUpdateFromPlayer GameHostState::sendMoveUpdate() {
  auto moveMessage = Gomoku::GomokuNetworking::gomokuQueue().getQueue();
  Gomoku::structs::GomokuEventReceiveCallback info =
      std::get<Gomoku::structs::GomokuEventReceiveCallback>(moveMessage.info);
  Gomoku::structs::GomokuData sendData{
      Gomoku::structs::GomokuCommunicationType::UNICAST,
      Gomoku::GomokuMessageStates::SENDING_MOVE,
      0,
      0,
      0,
      {}};
  memcpy(sendData.payload.data(), info.data.payload.data(),
         sizeof(Gomoku::structs::GomokuMoveUpdateFromPlayer));
  sendAll(sendData);
  xTaskNotifyGive(Gomoku::GomokuNetworking::gomokuNetworkingTask());
  auto data = info.data;
  return *reinterpret_cast<Gomoku::structs::GomokuMoveUpdateFromPlayer*>(
      &data.payload);
}

void GameHostState::sendAll(Gomoku::structs::GomokuData& sendData) {
  sendData.crc =
      esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const*>(&sendData),
                   Gomoku::consts::ESPNOW_SEND_LEN);
  for (const auto& mac : macAddresses_) {
    Gomoku::structs::GomokuDataWithRecipient finalMessage{mac, sendData};
    Gomoku::GomokuNetworking::sendingQueue().putQueue(finalMessage);
  }
}

bool GameHostState::updateBoard(
    Gomoku::structs::GomokuMoveUpdateFromPlayer nextMove) {
  Globals::game.gomokuBoard().markMove(
      Globals::game.gomokuBoard().player2Int(nextMove.player), nextMove.move);
  return Globals::game.gomokuBoard().isWinner();
}

void GameHostState::deinit() {}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
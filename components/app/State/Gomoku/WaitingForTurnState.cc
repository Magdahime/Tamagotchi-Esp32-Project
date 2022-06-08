#include "Gomoku/WaitingForTurnState.hpp"

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworking.hpp"

using namespace tamagotchi::App::GomokuNetworking;

namespace tamagotchi::App::State {

void WaitingForTurnState::handleEvent(Event::Event event) {}
void WaitingForTurnState::init() {}
void WaitingForTurnState::mainLoop() {
  ESP_LOGI(TAG_, "Waiting for my turn");
  structs::GomokuEvent msg;
  if (GomokuNetworking::GomokuNetworking::receiveQueue().getQueue(msg) ==
          pdPASS &&
      msg.macAddress == GomokuNetworking::GomokuNetworking::hostAddress()) {
    ESP_LOGI(TAG_, "Message from Game Host");
    auto gomokuData = GomokuNetworking::GomokuNetworking::unpackData(msg);
    auto& state = gomokuData.state;
    auto& payload = gomokuData.payload;
    if (state == GomokuNetworking::GomokuMessageStates::ERROR) return;
    switch (state) {
      case GomokuMessageStates::SENDING_ORDER:
        sendAck();
        ESP_LOGI(TAG_, "MY_TURN message");
        Globals::game.setNextState(StateType::PlayerTurn);
        break;

      case GomokuMessageStates::SENDING_MOVE_TO_PLAYERS:
        sendAck();
        ESP_LOGI(TAG_, "UPDATE_BOARD message.");
        updateBoard(reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(
            payload.data()));
        break;

      case GomokuMessageStates::END_OF_GAME:
        sendAck();
        ESP_LOGI(TAG_, "END_OF_GAME message.");
        Globals::game.setNextState(StateType::EndMiniGame);
        break;

      case GomokuMessageStates::SENDING_CONFIG:
        sendAck();
        updateGomokuInfo(payload);
      default:
        break;
    }
  }
}

void WaitingForTurnState::updateBoard(
    structs::GomokuMoveUpdateFromPlayer* nextMove) {
  Globals::game.gomokuBoard().markMove(nextMove->player, nextMove->move);
}

void WaitingForTurnState::updateGomokuInfo(gomoku_payload_array_t dataArray) {}

void WaitingForTurnState::sendAck() {
  ESP_LOGI(TAG_, "Sending ACK.");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::ACK,
                               0,
                               0,
                               {}};
  structs::GomokuDataWithRecipient finalMessage{
      GomokuNetworking::GomokuNetworking::gameHostAddress(), sendData};
  GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
}

void WaitingForTurnState::deinit() {}

}  // namespace tamagotchi::App::State
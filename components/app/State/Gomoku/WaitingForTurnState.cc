#include "Gomoku/WaitingForTurnState.hpp"

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworking.hpp"
#include "Shapes/EspGLRectangles.hpp"

using namespace tamagotchi::App::GomokuNetworking;

namespace tamagotchi::App::State {

void WaitingForTurnState::handleEvent(Event::Event event) {}
void WaitingForTurnState::init() {
  myTurn_ = false;
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "WAITING FOR OTHERS TO MOVE",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT / 2}},
      EspGL::colours::GREEN);
}
void WaitingForTurnState::mainLoop() {
  ESP_LOGI(TAG_, "Waiting for my turn");

  structs::GomokuEvent msg;
  if (GomokuNetworking::GomokuNetworking::receiveQueue().getQueue(
          msg, GomokuNetworking::consts::ESPNOW_SEND_DELAY) == pdPASS &&
      msg.macAddress == GomokuNetworking::GomokuNetworking::gameHostAddress()) {
    ESP_LOGI(TAG_, "Message from Game Host");
    auto gomokuData = GomokuNetworking::GomokuNetworking::unpackData(msg);
    auto& state = gomokuData.state;
    auto& payload = gomokuData.payload;
    if (state == GomokuNetworking::GomokuMessageStates::ERROR) return;
    switch (state) {
      case GomokuMessageStates::SENDING_ORDER:
        sendAck(gomokuData.magic);
        ESP_LOGI(TAG_, "MY_TURN message");
        myTurn_ = true;
        break;

      case GomokuMessageStates::SENDING_MOVE_TO_PLAYERS:
        sendAck(gomokuData.magic);
        ESP_LOGI(TAG_, "UPDATE_BOARD message.");
        updateBoard(reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(
            payload.data()));
        break;
      case GomokuMessageStates::END_OF_GAME:
        sendAck(gomokuData.magic);
        ESP_LOGI(TAG_, "END_OF_GAME message.");
        Globals::game.setNextState(StateType::EndMiniGame);
        break;

      case GomokuMessageStates::SENDING_COLOUR_CONFIG:
        sendAck(gomokuData.magic);
        ESP_LOGI(TAG_, "Colour config received from GameHost");
        updateGomokuPlayersColourInfo(payload);
      default:
        break;
    }
  }

  if (myTurn_ && GomokuNetworking::GomokuNetworking::receiveQueue().empty()) {
    Globals::game.setNextState(StateType::PlayerTurn);
  }

  ESP_LOGI(TAG_, "MY TURN %d", myTurn_);
  ESP_LOGI(TAG_,
           "GomokuNetworking::GomokuNetworking::receiveQueue().empty() %d",
           GomokuNetworking::GomokuNetworking::receiveQueue().empty());
  displayWaitingMessage();
}

void WaitingForTurnState::updateBoard(
    structs::GomokuMoveUpdateFromPlayer* nextMove) {
  Globals::game.gomokuBoard().markMove(nextMove->player, nextMove->move);
}

void WaitingForTurnState::updateGomokuPlayersColourInfo(
    gomoku_payload_array_t dataArray) {
  auto colourConfigPtr =
      reinterpret_cast<structs::ColourConfig*>(dataArray.data());
  for (auto& elem : colourConfigPtr->colour2Player) {
    Globals::game.gomokuBoard().player2Colour().emplace(
        std::make_pair(elem.player, elem.colourValue));
  }
}

void WaitingForTurnState::sendAck(uint32_t magic) {
  ESP_LOGI(TAG_, "Sending ACK.");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::ACK,
                               0,
                               magic,
                               {}};
  sendData.crc =
      esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const*>(&sendData),
                   GomokuNetworking::consts::ESPNOW_SEND_LEN);
  structs::GomokuDataWithRecipient finalMessage{
      GomokuNetworking::GomokuNetworking::gameHostAddress(), sendData};
  GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
}

void WaitingForTurnState::displayWaitingMessage() {
  const std::string dots(counter_, '.');
  ESP_LOGI(TAG_, "%s", dots.c_str());
  Globals::game.print(
      dots,
      {{0, (Game::consts::SCREEN_HEIGHT / 2)},
       {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN);
  counter_++;
  counter_ %= MAX_WAITING_DOTS;
  if (counter_ == 0) {
    EspGL::Rectangle<uint16_t>{{0, (Game::consts::SCREEN_HEIGHT / 2)},
                               Game::consts::SCREEN_WIDTH - 1,
                               (Game::consts::SCREEN_HEIGHT / 2) - 1,
                               Globals::defaultValues::BACKGROUND_COLOUR}
        .draw(Globals::game.screen());
  }
}

void WaitingForTurnState::deinit() {}

}  // namespace tamagotchi::App::State
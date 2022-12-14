#include "Gomoku/WaitingForTurnState.hpp"

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworking.hpp"
#include "Shapes/EspGLRectangles.hpp"

using namespace tamagotchi::App::GomokuNetworking;

namespace tamagotchi::App::State {

void WaitingForTurnState::handleEvent(Event::Event event) {}
void WaitingForTurnState::stateInit() {
  myTurn_ = false;
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "WAITING FOR OTHERS\nTO MOVE",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT / 2}},
      EspGL::colours::GREEN);
}
void WaitingForTurnState::mainLoop() {
  ESP_LOGI(TAG_, "Waiting for my turn");
  structs::GomokuEvent msg;
  if (GomokuNetworking::GomokuNetworking::hostQueue().getQueue(
          msg, GomokuNetworking::consts::ESPNOW_SEND_DELAY) == pdPASS) {
    ESP_LOGI(TAG_, "Message from " MACSTR, MAC2STR(msg.macAddress));
    auto gomokuData = GomokuNetworking::GomokuNetworking::unpackData(msg);
    const auto& state = gomokuData.state;
    auto& payload = gomokuData.payload;
    structs::GomokuMoveUpdateFromPlayer* newMove;
    if (state == GomokuNetworking::GomokuMessageStates::ERROR) {
      ESP_LOGE(TAG_, "ERROR MESSAGE");
      return;
    }
    switch (state) {
      case GomokuMessageStates::START_OF_GAME:
        ESP_LOGI(TAG_, "START GAME message");
        sendAck(GomokuNetworking::GomokuNetworking::gameHostAddress(),
                gomokuData.magic);
        break;

      case GomokuMessageStates::SENDING_ORDER:
        ESP_LOGI(TAG_, "SEND ORDER message");
        sendAck(GomokuNetworking::GomokuNetworking::gameHostAddress(),
                gomokuData.magic);
        mac_address_t receiverAddress;
        memcpy(receiverAddress.data(), payload.data(), ESP_NOW_ETH_ALEN);
        if (receiverAddress ==
                GomokuNetworking::GomokuNetworking::hostAddress() &&
            lastMyTurnMagic_ != gomokuData.magic) {
          lastMyTurnMagic_ = gomokuData.magic;
          ESP_LOGI(TAG_, "MY_TURN message");
          myTurn_ = true;
        }
        break;

      case GomokuMessageStates::SENDING_MOVE_TO_PLAYERS:
        sendAck(GomokuNetworking::GomokuNetworking::gameHostAddress(),
                gomokuData.magic);
        ESP_LOGI(TAG_, "UPDATE_BOARD message.");
        newMove = reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(
            payload.data());
        memcpy(receiverAddress.data(), newMove->player.data(),
               ESP_NOW_ETH_ALEN);
        if (receiverAddress !=
            GomokuNetworking::GomokuNetworking::hostAddress())
          updateBoard(reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(
              payload.data()));
        break;

      case GomokuMessageStates::END_OF_GAME:
        sendAck(GomokuNetworking::GomokuNetworking::gameHostAddress(),
                gomokuData.magic);
        ESP_LOGI(TAG_, "END_OF_GAME message.");
        Globals::game.setNextState(StateType::EndMiniGame);
        break;

      case GomokuMessageStates::SENDING_COLOUR_CONFIG:
        sendAck(GomokuNetworking::GomokuNetworking::gameHostAddress(),
                gomokuData.magic);
        ESP_LOGI(TAG_, "COLOUR_CONFIG message");
        updateGomokuPlayersColourInfo(payload);
        break;

      default:
        ESP_LOGE(TAG_, "UNKNOWN STATE: %d", state);
        sendAck(GomokuNetworking::GomokuNetworking::gameHostAddress(),
                gomokuData.magic);
        break;
    }
  }
  ESP_LOGI(TAG_, "MY TURN: %d", myTurn_);
  ESP_LOGI(TAG_, "QUEUE_EMPTY : %d",
           GomokuNetworking::GomokuNetworking::hostQueue().empty());
  ESP_LOGI(TAG_, "NUMBER OF ELEMENTS : %d",
           GomokuNetworking::GomokuNetworking::hostQueue().elementsCount());

  if (myTurn_ && GomokuNetworking::GomokuNetworking::hostQueue().empty()) {
    myTurn_ = false;
    Globals::game.setNextState(StateType::PlayerTurn);
  }
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
  auto& params = GomokuNetworking::GomokuNetworking::playersParams();
  auto& player2Colour = Globals::game.gomokuBoard().player2Colour();
  for (auto& elem : colourConfigPtr->colour2Player) {
    player2Colour.emplace(std::make_pair(elem.player, elem.colourValue));
    auto it = std::find_if(params.begin(), params.end(), [&](auto& pair) {
      return pair.first == elem.player;
    });
    (*it).second.setColour(EspGL::Colour16(elem.colourValue));
  }
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

void WaitingForTurnState::displayOrderMessage(mac_address_t nextPlayer) {
  ESP_LOGI(TAG_, "Display Order Message");
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "#########\nIt is another\nplayer turn!\n#########",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN);
  auto& params = GomokuNetworking::GomokuNetworking::playersParams();
  auto it = std::find_if(params.begin(), params.end(),
                         [&](auto pair) { return pair.first == nextPlayer; });
  (*it).second.setStart(EspGL::Vect2(0, Game::consts::SCREEN_HEIGHT / 2));
  (*it).second.draw(Globals::game.screen());
}

void WaitingForTurnState::deinit() {}

}  // namespace tamagotchi::App::State
#include "Gomoku/PlayerTurnState.hpp"

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworkingConf.hpp"
using namespace tamagotchi::App::GomokuNetworking;
namespace tamagotchi::App::State {

PlayerTurnState::PlayerTurnState()
    : rowCounter_(0),
      columnCounter_(0),
      gomokuBoard_(Globals::game.gomokuBoard()) {}

void PlayerTurnState::handleEvent(Event::Event event) {
  gpio_num_t pressedButton;
  switch (event.type_) {
    case Event::EventTypes::gpio:
      pressedButton = *reinterpret_cast<gpio_num_t*>(event.data_);
      handleGpioInput(pressedButton);
      break;

    case Event::EventTypes::terminate:
      ESP_LOGI(TAG_, "TERMINATE EVENT");
      Globals::game.setNextState(StateType::End);
      break;

    default:
      break;
  }
}

void PlayerTurnState::stateInit() {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "#########\nIt is your\nturn now!:\n#########",
      {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      EspGL::colours::GREEN);
  Globals::game.pet().setStart({0, Game::consts::SCREEN_HEIGHT / 2});
  Globals::game.pet().draw(Globals::game.screen());
  EspGL::delay(3000);
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  gomokuBoard_.board()[columnCounter_ * gomokuBoard_.width() + rowCounter_]
      .highlight();
  redrawScreen();
}

void PlayerTurnState::mainLoop() {
  Event::Event event = tamagotchi::App::Globals::game.eventQueue().getQueue(
      consts::USER_INPUT_WAIT_TIME);
  if (!event.empty()) {
    handleEvent(event);
    tamagotchi::App::Globals::game.eventQueue().clearQueue();
    redrawScreen();
  }
}

void PlayerTurnState::deinit() {}

void PlayerTurnState::handleGpioInput(gpio_num_t pressedButton) {
  ESP_LOGI(TAG_, "handleGpioInput.");
  ESP_LOGI(TAG_, "ROW COUNTER %d, COLUMN COUNTER %d, TILE %d ", rowCounter_,
           columnCounter_, columnCounter_ * gomokuBoard_.width() + rowCounter_);
  gomokuBoard_.board()[columnCounter_ * gomokuBoard_.width() + rowCounter_]
      .dehighlight();
  switch (pressedButton) {
    case Gpio::GpioInputs::GPIO_LEFT:
      ESP_LOGI(TAG_, "LEFT.");
      rowCounter_ =
          rowCounter_ > 0 ? rowCounter_ - 1 : gomokuBoard_.width() - 1;
      break;

    case Gpio::GpioInputs::GPIO_RIGHT:
      ESP_LOGI(TAG_, "RIGHT.");
      rowCounter_ =
          rowCounter_ < gomokuBoard_.width() - 1 ? rowCounter_ + 1 : 0;
      break;

    case Gpio::GpioInputs::GPIO_MIDDLE:
      ESP_LOGI(TAG_, "MIDDLE.");
      if (gomokuBoard_
              .board()[columnCounter_ * gomokuBoard_.width() + rowCounter_]
              .empty()) {
        auto newMove = BoardCoordinate{rowCounter_, columnCounter_};
        Globals::game.gomokuBoard().markMove(
            GomokuNetworking::GomokuNetworking::hostAddress(), newMove);
        sendMoveUpdateToHost(newMove);
      }

      else
        displayErrorMessage("THIS TILE IS ALREADY\nTAKEN!!!",
                            EspGL::Vect2(0, 0));
      break;

    case Gpio::GpioInputs::GPIO_UP:
      ESP_LOGI(TAG_, "UP.");
      columnCounter_ =
          columnCounter_ > 0 ? columnCounter_ - 1 : gomokuBoard_.height() - 1;
      break;

    case Gpio::GpioInputs::GPIO_DOWN:
      ESP_LOGI(TAG_, "DOWN.");
      columnCounter_ =
          columnCounter_ < gomokuBoard_.height() - 1 ? columnCounter_ + 1 : 0;
      break;

    default:
      break;
  }
  gomokuBoard_.board()[columnCounter_ * gomokuBoard_.width() + rowCounter_]
      .highlight();
  ESP_LOGI(TAG_, "ROW COUNTER %d, COLUMN COUNTER %d, TILE %d ", rowCounter_,
           columnCounter_, columnCounter_ * gomokuBoard_.width() + rowCounter_);
}

void PlayerTurnState::redrawScreen() {
  ESP_LOGD(TAG_, "RedrawingScreen.");
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  gomokuBoard_.draw(Globals::game.screen());
}

void PlayerTurnState::sendMoveUpdateToHost(BoardCoordinate move) {
  ESP_LOGI(TAG_, "Sending move update to host");
  auto myMac = GomokuNetworking::GomokuNetworking::hostAddress();
  structs::GomokuMoveUpdateFromPlayer updateMove{myMac, move};
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::SENDING_MOVE_TO_HOST,
                               0,
                               0,
                               {}};
  memcpy(sendData.payload.data(), reinterpret_cast<uint8_t*>(&updateMove),
         sizeof(structs::GomokuMoveUpdateFromPlayer));

  if (myMac != GomokuNetworking::GomokuNetworking::gameHostAddress()) {
    sendData.magic = esp_random();
    sendData.crc =
        esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const*>(&sendData),
                     GomokuNetworking::consts::ESPNOW_SEND_LEN);
    structs::GomokuDataWithRecipient finalMessage{
        GomokuNetworking::GomokuNetworking::gameHostAddress(), sendData};
    GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
    Globals::game.setNextState(StateType::WaitingForTurn);
  } else {
    sendData.state = GomokuMessageStates::SENDING_MOVE_TO_PLAYERS;
    for (auto mac : GomokuNetworking::GomokuNetworking::playersMacs()) {
      sendData.magic = esp_random();
      sendData.crc =
          esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const*>(&sendData),
                       GomokuNetworking::consts::ESPNOW_SEND_LEN);
      structs::GomokuDataWithRecipient finalMessage{mac, sendData};
      GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
    }
    Globals::game.setNextState(StateType::GameHostDuties);
  }
}

}  // namespace tamagotchi::App::State
#include "State.hpp"

#include "Game.hpp"
#include "Globals.hpp"

namespace tamagotchi {
namespace App {
namespace State {

using namespace tamagotchi::App::GomokuNetworking;

void State::init() {
  Globals::game.eventQueue().clearQueue();
  stateInit();
}
void State::loop() { mainLoop(); }

void State::displayErrorMessage(std::string message, EspGL::Vect2 placing) {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      message,
      {placing, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
      Game::consts::ERROR_COLOUR);
  EspGL::delay(4000);
}

void State::sendAck(mac_address_t player, uint32_t magic) {
  ESP_LOGI("State", "Sending ACK.");
  structs::GomokuData sendData{structs::GomokuCommunicationType::UNICAST,
                               GomokuMessageStates::ACK,
                               0,
                               magic,
                               {}};
  sendData.crc =
      esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const*>(&sendData),
                   GomokuNetworking::consts::ESPNOW_SEND_LEN);
  structs::GomokuDataWithRecipient finalMessage{player, sendData};
  GomokuNetworking::GomokuNetworking::sendingQueue().putQueue(finalMessage);
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi

#include "GameHostState.hpp"

namespace tamagotchi {
namespace App {
namespace State {

void GameHostState::handleEvent(Event::Event event) {}
void GameHostState::init() {}
void GameHostState::mainLoop() {
  bool winner = false;

  while (!winner) {
    makeMove();
    sendMoveUpdate();
    for (const auto& player : macAddresses_) {
      sendNotificationAboutNextPlayer(player);
      waitForPlayerMove(player);
      sendMoveUpdate();
    }
  }
}
void GameHostState::deinit() {}


void GameHostState::sendMoveUpdate() {}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
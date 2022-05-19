#include "Gomoku/WaitingForTurnState.hpp"

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworking.hpp"

using namespace tamagotchi::App::Gomoku;

namespace tamagotchi::App::State {

void WaitingForTurnState::handleEvent(Event::Event event) {}
void WaitingForTurnState::init() {}
void WaitingForTurnState::mainLoop() {
  structs::GomokuEvent msg;
  if (GomokuNetworking::receiveQueue().getQueue(msg) == pdPASS &&
      msg.macAddress == GomokuNetworking::hostAddress()) {
    auto state =
        std::get<structs::GomokuEventReceiveCallback>(msg.info).data.state;

    auto payload = GomokuNetworking::unpackData(msg);
    if (payload.empty()) return;
    if (state == GomokuMessageStates::SENDING_ORDER) {
      Globals::game.setNextState(StateType::PlayerTurn);
    } else if (state == GomokuMessageStates::SENDING_MOVE) {
      updateBoard(reinterpret_cast<structs::GomokuMoveUpdateFromPlayer*>(
          payload.data()));
    } else if (state == GomokuMessageStates::END_OF_GAME) {
      // WHAT HAPPENS AT THE END OF THE GAME
      //  updateBoard(reinterpret_cast<tructs::GomokuMoveUpdateFromPlayer*>());
    }
  }
}

void updateBoard(structs::GomokuMoveUpdateFromPlayer* nextMove) {
  Globals::game.gomokuBoard().markMove(
      Globals::game.gomokuBoard().player2Int(nextMove->player), nextMove->move);
}

void WaitingForTurnState::deinit() {}

}  // namespace tamagotchi::App::State
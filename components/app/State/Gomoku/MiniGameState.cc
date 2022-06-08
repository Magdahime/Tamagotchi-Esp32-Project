#include "Gomoku/MiniGameState.hpp"

#include "ColourProvider.hpp"
#include "Game.hpp"
#include "Globals.hpp"

namespace tamagotchi {
namespace App {
namespace State {

using namespace tamagotchi::App::GomokuNetworking;

MiniGameState::MiniGameState() {}

void MiniGameState::handleEvent(Event::Event event) {}

void MiniGameState::init() {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.print(
      "## WELCOME TO GOMOKU ##",
      {{0, 0},
       {Globals::game.screen().width(), Globals::game.screen().height()}},
      EspGL::colours::GREEN);

  Globals::game.print(
      "Finding players...",
      {{0, 100},
       {Globals::game.screen().width(), Globals::game.screen().height()}},
      EspGL::colours::GREEN);
  EspGL::delay(3000);
  startNetworkingTask();
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
}

void MiniGameState::mainLoop() {
  if (GomokuNetworking::GomokuNetworking::gameHostAddress() ==
      GomokuNetworking::GomokuNetworking::hostAddress()) {
    Globals::game.print(
        "YOU ARE THE HOST",
        {{0, 0},
         {Globals::game.screen().width(), Globals::game.screen().height()}},
        EspGL::colours::GREEN);
    EspGL::delay(1000);
    Globals::game.setNextState(StateType::GameHostDuties);
  } else {
    Globals::game.print(
        "YOU ARE NORMAL PLAYER",
        {{0, 0},
         {Globals::game.screen().width(), Globals::game.screen().height()}},
        EspGL::colours::GREEN);
    EspGL::delay(1000);
    Globals::game.setNextState(StateType::WaitingForTurn);
  }
}

void MiniGameState::deinit() {}

void MiniGameState::startNetworkingTask() {
  GomokuNetworking::GomokuNetworking::init();
  GomokuNetworking::GomokuNetworking::run();
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi
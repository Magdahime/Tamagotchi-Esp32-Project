#include "Gomoku/EndGameState.hpp"

#include <sstream>
#include <string>

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworkingConf.hpp"
using namespace tamagotchi::App::Gomoku;

namespace tamagotchi::App::State {

void EndGameState::handleEvent(Event::Event event) {}
void EndGameState::init() {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  if (Globals::game.gomokuBoard().macWinner() ==
      GomokuNetworking::hostAddress()) {
    std::string message = "YOU ARE THE WINNER!\nCongratulations!";
    Globals::game.print(
        message,
        {{0, 100},
         {Globals::game.screen().width(), Globals::game.screen().height()}},
        EspGL::colours::GREEN);
  } else {
    constexpr int MAC_SIZE = 18;
    std::string addr(MAC_SIZE, '\0');
    std::snprintf(addr.data(), addr.size(), MACSTR,
                  MAC2STR(Globals::game.gomokuBoard().macWinner()));
    std::string message = "Winner is player " +
                          std::to_string(Globals::game.gomokuBoard().winner()) +
                          " (" + addr +
                          ")"
                          "!\nCongratulations!";
    Globals::game.print(
        message,
        {{0, 100},
         {Globals::game.screen().width(), Globals::game.screen().height()}},
        EspGL::colours::GREEN);
  }
  EspGL::delay(3000);
  Globals::game.setNextState(StateType::MiniGame);
}
void EndGameState::mainLoop() {}
void EndGameState::deinit() {}

}  // namespace tamagotchi::App::State
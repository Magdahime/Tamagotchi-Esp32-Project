#include "Gomoku/EndGameState.hpp"

#include <sstream>
#include <string>

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworkingConf.hpp"
using namespace tamagotchi::App::GomokuNetworking;
namespace tamagotchi::App::State {

void EndGameState::handleEvent(Event::Event event) {}
void EndGameState::init() {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  if (Globals::game.gomokuBoard().winner() ==
      GomokuNetworking::GomokuNetworking::hostAddress()) {
    std::string message = "YOU ARE THE WINNER!\nCongratulations!";
    Globals::game.print(
        message,
        {{0, 100},
         {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
        EspGL::colours::GREEN);
  } else {
    constexpr int MAC_SIZE = 18;
    std::string addr(MAC_SIZE, '\0');
    std::snprintf(addr.data(), addr.size(), MACSTR,
                  MAC2STR(Globals::game.gomokuBoard().winner()));
    std::string message = "Winner is player: (" + addr +
                          ")"
                          "!\nCongratulations!";
    Globals::game.print(
        message,
        {{0, 100},
         {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
        EspGL::colours::GREEN);
  }
  EspGL::delay(3000);
  Globals::game.setNextState(StateType::MiniGame);
}
void EndGameState::mainLoop() {}
void EndGameState::deinit() {}

}  // namespace tamagotchi::App::State
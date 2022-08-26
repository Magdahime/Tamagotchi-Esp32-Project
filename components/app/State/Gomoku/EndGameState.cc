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
  auto winner = Globals::game.gomokuBoard().winner();
  ESP_LOGI(TAG_, "Globals::game.gomokuBoard().winner(): " MACSTR,
           MAC2STR(winner));
  ESP_LOGI(TAG_, "GomokuNetworking::GomokuNetworking::hostAddress(): " MACSTR,
           MAC2STR(GomokuNetworking::GomokuNetworking::hostAddress()));
  if (winner == GomokuNetworking::GomokuNetworking::hostAddress()) {
    std::string message = "YOU ARE THE WINNER!\nCongratulations!";
    Globals::game.print(
        message,
        {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
        EspGL::colours::GREEN);
  } else if (!Globals::game.gomokuBoard().full()) {
    std::string message = "Winner is player:";
    Globals::game.print(
        message,
        {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
        EspGL::colours::GREEN);
    auto& params = GomokuNetworking::GomokuNetworking::playersParams();
    auto it = std::find_if(params.begin(), params.end(),
                           [&](auto pair) { return pair.first == winner; });
    (*it).second.setStart(EspGL::Vect2(0, Game::consts::SCREEN_HEIGHT / 2));
    (*it).second.draw(Globals::game.screen());

  } else {
    std::string message = "Draw!\nBetter luck next\ntime!";
    Globals::game.print(
        message,
        {{0, 0}, {Game::consts::SCREEN_WIDTH, Game::consts::SCREEN_HEIGHT}},
        EspGL::colours::GREEN);
  }
  EspGL::delay(3000);
  Globals::game.setNextState(StateType::MainMenu);
}
void EndGameState::mainLoop() {}
void EndGameState::deinit() {
  vTaskDelete(GomokuNetworking::GomokuNetworking::gomokuNetworkingTask());
}

}  // namespace tamagotchi::App::State
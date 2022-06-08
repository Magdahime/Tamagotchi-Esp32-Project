#pragma once

#include <stdint.h>

#include "GomokuNetworkingConf.hpp"

namespace tamagotchi::App::Gomoku {

namespace consts {

constexpr int POINT_COUNT = 3;
constexpr int INITIAL_VALUE = 0;
constexpr int GOMOKU_PAWN_SIZE = 25;  // in pixels
constexpr int GOMOKU_LINE_WIDTH = 2;  // in pixels
constexpr auto LINE_COLOUR = EspGL::colours::WHITE;
constexpr auto HIGHLIGHT_COLOUR = EspGL::colours::YELLOW;

}  // namespace consts

}  // namespace tamagotchi::App::Gomoku
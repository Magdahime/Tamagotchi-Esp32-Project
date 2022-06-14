#pragma once
#include "EspGLUtils.hpp"
namespace tamagotchi::App::Game {

namespace consts {
constexpr int EVENT_QUEUE_SIZE = 10;
constexpr int PET_SCALE = 5;
constexpr int GOMOKU_WIDTH = 3;
constexpr int GOMOKU_HEIGHT = 3;

constexpr int SCREEN_WIDTH = 240;
constexpr int SCREEN_HEIGHT = 320;


constexpr EspGL::Colour<uint16_t> ERROR_COLOUR = EspGL::colours::RED;

}  // namespace consts

}  // namespace tamagotchi::App::Game
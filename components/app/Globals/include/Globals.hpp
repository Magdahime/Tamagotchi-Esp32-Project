#pragma once

#include "EspGLUtils.hpp"
#include "SPIFFSDriver.hpp"

namespace tamagotchi {

namespace App {
namespace Game {
class Game;
}
namespace Globals {

namespace defaultValues {
constexpr char PET_COMPONENTS_PATH[] = "petComponents.pbm";
constexpr char SERIALIZED_PET_PATH[] = "pet.ser";
constexpr char FONT_FILE_PATH[] = "fullFont.pbm";
constexpr char ICONS_FILE_PATH[] = "icons.pbm";
constexpr auto BACKGROUND_COLOUR = EspGL::colours::BLACK;
}  // namespace defaultValues

extern SPIFFS::SPIFFSDriver spiffsDriver;
extern Game::Game game;

}  // namespace Globals
}  // namespace App
}  // namespace tamagotchi
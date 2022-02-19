#pragma once

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
}  // namespace defaultValues

extern SPIFFS::SPIFFSDriver spiffsDriver;
extern Game::Game game;

}  // namespace Globals
}  // namespace App
}  // namespace tamagotchi
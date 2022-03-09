#include "MainMenuState.hpp"

#include <memory>

#include "BitmapLoader.hpp"
#include "Drawable.hpp"
#include "Game.hpp"
#include "Globals.hpp"

namespace tamagotchi {
namespace App {
namespace State {

MainMenuState::MainMenuState() { deserializeIcons(); }

void MainMenuState::handleEvent(Event::Event Event) {}

void MainMenuState::init() {
  Globals::game.screen().fill(EspGL::colours::BLACK);
}
void MainMenuState::mainLoop() {}
void MainMenuState::deinit() {}

void MainMenuState::deserializeIcons() {
  BitmapLoader::BitmapLoader bitmapLoader(
      Globals::spiffsDriver.getFileDescriptor(
          Globals::defaultValues::ICONS_FILE_PATH));
  bitmapLoader.parseMagicNumber();
  auto dimensions = bitmapLoader.parseDimensions();
  auto bitmapNumber = bitmapLoader.parseBitmapNumber();
  labels_ = bitmapLoader.parseSymbols(bitmapNumber);
  std::vector<EspGL::Bitmap> bitmaps;
  for (auto i = 0; i < bitmapNumber; ++i) {
    bitmaps.push_back(EspGL::Bitmap(
        dimensions.first, dimensions.second,
        bitmapLoader.parseBitmap(dimensions.first, dimensions.second)));
  }

  EspGL::Point nextIconStart = {consts::START_ICONS.first,
                                consts::START_ICONS.second};
  for (auto i = 0; i < bitmapNumber; i++) {
    drawables_.emplace(labels_[i],
                       std::make_unique<EspGL::BitmapDrawable<uint16_t>>(
                           bitmaps[i], nextIconStart, consts::ICONS_SCALE,
                           consts::ICON_COLOUR));
    nextIconStart = {nextIconStart.x_ +
                         bitmaps[i].sizeX() * consts::ICONS_SCALE +
                         consts::ICONS_SPACING,
                     nextIconStart.y_};
    if (nextIconStart.x_ > Globals::game.screen().width()) {
      ESP_LOGE(TAG_, "Too many icons!");
      break;
    }
  }
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi

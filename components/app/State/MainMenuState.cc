#include "MainMenuState.hpp"

#include <stdint.h>

#include <algorithm>
#include <iterator>
#include <memory>

#include "BitmapLoader.hpp"
#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Event.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Gpioconf.hpp"
#include "Shapes/EspGLRectangles.hpp"

namespace tamagotchi {
namespace App {
namespace State {

MainMenuState::MainMenuState() {
  deserializeIcons();
  std::transform(drawables_.begin(), drawables_.end(),
                 std::back_inserter(hitboxes_),
                 [](auto& drawable_pair) -> EspGL::Hitbox {
                   auto& [id, drawable] = drawable_pair;
                   return drawable->hitbox();
                 });
}

void MainMenuState::handleEvent(Event::Event event) {
  int pressedButton = 0;
  switch (event.type_) {
    case Event::EventTypes::gpio:
      ESP_LOGI(TAG_, "GPIO EVENT");
      pressedButton = *reinterpret_cast<int*>(event.data_);
      handleGpioInput(pressedButton);
      break;

    case Event::EventTypes::terminate:
      ESP_LOGI(TAG_, "TERMINATE EVENT");
      Globals::game.setNextState(StateType::End);
      break;

    case Event::EventTypes::espNow:
      ESP_LOGI(TAG_, "ESPNOW EVENT");
      break;
    default:
      break;
  }
}

void MainMenuState::init() {
  Globals::game.screen().fill(EspGL::colours::BLACK);
  ESP_LOGI(TAG_, "Drawing icons.");
  for (auto const& [key, val] : drawables_) {
    val->draw(Globals::game.screen());
  }
  ESP_LOGI(TAG_, "Drawing pet.");
  Globals::game.pet().draw(Globals::game.screen());
}
void MainMenuState::mainLoop() {
  StateUtils::movePet(hitboxes_);
  EspGL::delay(2000);
}
void MainMenuState::deinit() {}

void MainMenuState::deserializeIcons() {
  ESP_LOGI(TAG_, "Deserializing icons.");
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

  EspGL::Vect2 nextIconStart = {consts::START_ICONS.first,
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
  iconPointer_ = labels_.begin();
}

void MainMenuState::shiftIconPointer(MainMenuState::Direction direction) {
  ESP_LOGI(TAG_, "Shifting Icon Pointer.");
  auto bitmapPointer = static_cast<EspGL::BitmapDrawable<uint16_t>*>(
      drawables_[*iconPointer_].get());
  auto currentColour = bitmapPointer->colour();
  bitmapPointer->setBackground(currentColour);
  bitmapPointer->setColour(
      EspGL::Colour<uint16_t>(currentColour.getNegativeColourValue()));
  drawables_[*iconPointer_]->redraw(Globals::game.screen());
  iconPointer_++;
  if (iconPointer_ == labels_.end()) {
    iconPointer_ = labels_.begin();
  }
}

void MainMenuState::handleGpioInput(int pressedButton) {
  switch (pressedButton) {
    case static_cast<int>(Gpio::GpioInputs::GPIO_LEFT):
      shiftIconPointer(MainMenuState::Direction::BACKWARDS);
      break;

    case static_cast<int>(Gpio::GpioInputs::GPIO_RIGHT):
      shiftIconPointer(MainMenuState::Direction::FORWARDS);
      break;

    default:
      break;
  }
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi

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
  stateTransitions_.resize(labels_.size());
  std::fill(stateTransitions_.begin(), stateTransitions_.end(),
            StateType::MainMenu);
  stateTransitions_[2] = StateType::MiniGame;
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

void MainMenuState::stateInit() {
  Globals::game.screen().fill(Globals::defaultValues::BACKGROUND_COLOUR);
  ESP_LOGI(TAG_, "Drawing icons.");
  for (auto const& [key, val] : drawables_) {
    val->draw(Globals::game.screen());
  }
  ESP_LOGI(TAG_, "Drawing pet.");
  Globals::game.pet().draw(Globals::game.screen());
}
void MainMenuState::mainLoop() {
  StateUtils::movePet(hitboxes_);
  Event::Event event = tamagotchi::App::Globals::game.eventQueue().getQueue(
      consts::USER_INPUT_WAIT_TIME);
  if (!event.empty()) {
    handleEvent(event);
  }
  tamagotchi::App::Globals::game.eventQueue().clearQueue();
  EspGL::delay(1000);
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
    if (nextIconStart.x_ > Game::consts::SCREEN_WIDTH) {
      ESP_LOGE(TAG_, "Too many icons!");
      break;
    }
  }
  currentSelection_ = 0;
  lastSelection_ = 0;
}

void MainMenuState::shiftIconPointer(MainMenuState::Direction direction) {
  ESP_LOGI(TAG_, "Shifting Icon Pointer.");
  if (direction == MainMenuState::Direction::FORWARDS) {
    currentSelection_++;
    if (currentSelection_ == labels_.size()) {
      currentSelection_ = 0;
    }
  } else {
    currentSelection_--;
    if (currentSelection_ < 0) {
      currentSelection_ = labels_.size() - 1;
    }
  }

  auto lastSel = static_cast<EspGL::BitmapDrawable<uint16_t>*>(
      drawables_[labels_[lastSelection_]].get());
  lastSel->setBackground(Globals::defaultValues::BACKGROUND_COLOUR);
  lastSel->setColour(EspGL::Colour<uint16_t>(consts::ICON_COLOUR));
  drawables_[labels_[lastSelection_]]->redraw(Globals::game.screen());

  auto currSel = static_cast<EspGL::BitmapDrawable<uint16_t>*>(
      drawables_[labels_[currentSelection_]].get());
  currSel->setBackground(consts::ICON_COLOUR);
  currSel->setColour(
      EspGL::Colour<uint16_t>(Globals::defaultValues::BACKGROUND_COLOUR));
  drawables_[labels_[currentSelection_]]->redraw(Globals::game.screen());

  lastSelection_ = currentSelection_;
}

void MainMenuState::handleGpioInput(int pressedButton) {
  ESP_LOGI(TAG_, "handleGpioInput.");
  switch (pressedButton) {
    case static_cast<int>(Gpio::GpioInputs::GPIO_LEFT):
      ESP_LOGI(TAG_, "LEFT.");
      shiftIconPointer(MainMenuState::Direction::BACKWARDS);
      break;

    case static_cast<int>(Gpio::GpioInputs::GPIO_RIGHT):
      ESP_LOGI(TAG_, "RIGHT.");
      shiftIconPointer(MainMenuState::Direction::FORWARDS);
      break;

    case static_cast<int>(Gpio::GpioInputs::GPIO_MIDDLE):
      ESP_LOGI(TAG_, "MIDDLE.");
      Globals::game.setNextState(stateTransitions_[currentSelection_]);
      break;

    default:
      break;
  }
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi

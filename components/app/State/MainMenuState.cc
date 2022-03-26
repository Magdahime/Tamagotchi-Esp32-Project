#include "MainMenuState.hpp"

#include <stdint.h>

#include <memory>

#include "BitmapLoader.hpp"
#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Event.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Shapes/EspGLRectangles.hpp"

namespace tamagotchi {
namespace App {
namespace State {

MainMenuState::MainMenuState() { deserializeIcons(); }

bool MainMenuState::checkCollision(EspGL::Point newPoint) {
  if (drawables_.size() == 0) {
    return false;
  }
  auto checkIfInside = [&](EspGL::Point leftUpper, EspGL::Point rightLower,
                           EspGL::Point tested) {
    return tested.x_ >= leftUpper.x_ and tested.x_ < rightLower.x_ and
           tested.y_ >= leftUpper.y_ and tested.y_ < rightLower.y_;
  };
  for (auto const& [key, val] : drawables_) {
    auto hitbox = val->getHitbox();
    if (checkIfInside(hitbox.first, hitbox.second, newPoint) == true) {
      return true;
    }
  }
  return false;
}

void MainMenuState::handleEvent(Event::Event event) {
  // int pressedButton = 0;
  switch (event.type_) {
    case Event::EventTypes::gpio:
      // pressedButton = *reinterpret_cast<int*>(event.data_);
      break;

    case Event::EventTypes::terminate:
      Globals::game.setNextState(StateType::End);
      break;

    case Event::EventTypes::espNow:
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
  movePet();
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
  iconPointer_ = labels_.begin();
}

void MainMenuState::movePet() {
  auto newCoordinate = Globals::game.pet().start();
  do {
    int stepY = esp_random() % consts::STEP_Y;
    int stepX = esp_random() % consts::STEP_X;
    newCoordinate.y_ +=
        esp_random() % consts::THRESHHOLD >= 0.5 * consts::THRESHHOLD ? stepY
                                                                      : -stepY;
    newCoordinate.x_ +=
        esp_random() % consts::THRESHHOLD >= 0.5 * consts::THRESHHOLD ? stepX
                                                                      : -stepX;
  } while (checkCollision(newCoordinate));
  Globals::game.pet().setStart(newCoordinate);
  Globals::game.pet().redraw(Globals::game.screen(),
                             Globals::defaultValues::BACKGROUND_COLOUR);
}

void MainMenuState::shiftIconPointer() {
  auto bitmapPointer = static_cast<EspGL::BitmapDrawable<uint16_t>*>(
      drawables_[*iconPointer_].get());
  auto currentColour = bitmapPointer->colour();
  bitmapPointer->setBackground(currentColour);
  bitmapPointer->setColour(
      EspGL::Colour<uint16_t>(currentColour.getNegativeColourValue()));
  drawables_[*iconPointer_]->redraw(Globals::game.screen(),
                                    Globals::defaultValues::BACKGROUND_COLOUR);
  iconPointer_++;
  if (iconPointer_ == labels_.end()) {
    iconPointer_ = labels_.begin();
  }
}

}  // namespace State
}  // namespace App
}  // namespace tamagotchi

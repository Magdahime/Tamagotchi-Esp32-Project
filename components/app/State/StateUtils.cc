#include "StateUtils.hpp"

#include <utility>
#include <vector>

#include "EspGLUtils.hpp"
#include "Game.hpp"

namespace tamagotchi {
namespace App {
namespace State {
namespace StateUtils {

bool checkCollision(
    std::pair<EspGL::Point, EspGL::Point> newHitbox,
    const std::vector<std::pair<EspGL::Point, EspGL::Point>>& hitboxes) {
  auto checkIfInside = [&](EspGL::Point leftUpper, EspGL::Point rightLower,
                           EspGL::Point tested) -> bool {
    return tested.x_ >= leftUpper.x_ and tested.x_ <= rightLower.x_ and
           tested.y_ >= leftUpper.y_ and tested.y_ <= rightLower.y_;
  };

  auto checkIfOutOfBound =
      [&](std::pair<EspGL::Point, EspGL::Point> hitbox) -> bool {
    return hitbox.first.x_ < 0 or hitbox.first.y_ < 0 or
           hitbox.second.x_ > Globals::game.screen().width() or
           hitbox.second.y_ > Globals::game.screen().height();
  };

  if (checkIfOutOfBound(newHitbox)) return true;
  if (hitboxes.size() == 0) return false;

  for (auto const& hitbox : hitboxes) {
    if (checkIfInside(hitbox.first, hitbox.second, newHitbox.first) == true) {
      return true;
    }
  }
  return false;
}

void movePet(
    const std::vector<std::pair<EspGL::Point, EspGL::Point>>& hitboxes) {
  auto oldPetHitbox = Globals::game.pet().hitbox();
  EspGL::Point newCoordinate{oldPetHitbox.first};

  newCoordinate = Globals::game.pet().start();
  int stepY = esp_random() % consts::STEP_Y;
  int stepX = esp_random() % consts::STEP_X;
  newCoordinate.y_ +=
      esp_random() % consts::THRESHHOLD >= 0.5 * consts::THRESHHOLD ? stepY
                                                                    : -stepY;
  newCoordinate.x_ +=
      esp_random() % consts::THRESHHOLD >= 0.5 * consts::THRESHHOLD ? stepX
                                                                    : -stepX;
  ESP_LOGI("StateUtils", "Trying to move pet: (%d, %d).", newCoordinate.x_,
           newCoordinate.y_);

  auto newHitbox = std::make_pair(
      newCoordinate,
      EspGL::Point(oldPetHitbox.second.x_ +
                       (Globals::game.pet().start().x_ - newCoordinate.x_),
                   oldPetHitbox.second.y_ +
                       (Globals::game.pet().start().y_ - newCoordinate.y_)));
  if (checkCollision(newHitbox, hitboxes)) {
    newCoordinate = {Globals::game.screen().width() / 2,
                     Globals::game.screen().height() / 2};
  }
  ESP_LOGI("StateUtils", "Moved pet to: (%d, %d).", newCoordinate.x_,
           newCoordinate.y_);
  Globals::game.pet().erase(Globals::game.screen(),
                            Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.pet().setStart(newCoordinate);
  Globals::game.pet().redraw(Globals::game.screen());
}

void movePet(
    std::pair<EspGL::Point, EspGL::Point> newHitbox,
    const std::vector<std::pair<EspGL::Point, EspGL::Point>>& hitboxes) {
  if (!checkCollision(newHitbox, hitboxes)) {
    Globals::game.pet().erase(Globals::game.screen(),
                              Globals::defaultValues::BACKGROUND_COLOUR);
    Globals::game.pet().setStart(newHitbox.first);
    Globals::game.pet().redraw(Globals::game.screen());
  }
}
}  // namespace StateUtils
}  // namespace State
}  // namespace App
}  // namespace tamagotchi
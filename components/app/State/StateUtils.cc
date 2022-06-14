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
    EspGL::Hitbox newHitbox,
    const std::vector<EspGL::Hitbox>& hitboxes) {
  auto checkIfInside = [&](EspGL::Vect2 leftUpper, EspGL::Vect2 rightLower,
                           EspGL::Vect2 tested) -> bool {
    return tested.x_ >= leftUpper.x_ and tested.x_ <= rightLower.x_ and
           tested.y_ >= leftUpper.y_ and tested.y_ <= rightLower.y_;
  };

  auto checkIfOutOfBound =
      [&](EspGL::Hitbox hitbox) -> bool {
    return hitbox.first.x_ < 0 or hitbox.first.y_ < 0 or
           hitbox.second.x_ > Game::consts::SCREEN_WIDTH or
           hitbox.second.y_ > Game::consts::SCREEN_HEIGHT;
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
    const std::vector<EspGL::Hitbox>& hitboxes) {
  auto oldPetHitbox = Globals::game.pet().hitbox();
  EspGL::Vect2 newCoordinate{oldPetHitbox.first};

  newCoordinate = Globals::game.pet().start();
  int stepY = esp_random() % consts::STEP_Y;
  int stepX = esp_random() % consts::STEP_X;
  newCoordinate.y_ +=
      esp_random() % consts::THRESHOLD >= 0.5 * consts::THRESHOLD ? stepY
                                                                    : -stepY;
  newCoordinate.x_ +=
      esp_random() % consts::THRESHOLD >= 0.5 * consts::THRESHOLD ? stepX
                                                                    : -stepX;
  ESP_LOGD("StateUtils", "Trying to move pet: (%d, %d).", newCoordinate.x_,
           newCoordinate.y_);

  auto newHitbox = std::make_pair(
      newCoordinate,
      EspGL::Vect2(oldPetHitbox.second.x_ +
                       (Globals::game.pet().start().x_ - newCoordinate.x_),
                   oldPetHitbox.second.y_ +
                       (Globals::game.pet().start().y_ - newCoordinate.y_)));
  if (checkCollision(newHitbox, hitboxes)) {
    newCoordinate = {Game::consts::SCREEN_WIDTH / 2,
                     Game::consts::SCREEN_HEIGHT / 2};
  }
  ESP_LOGD("StateUtils", "Moved pet to: (%d, %d).", newCoordinate.x_,
           newCoordinate.y_);
  Globals::game.pet().erase(Globals::game.screen(),
                            Globals::defaultValues::BACKGROUND_COLOUR);
  Globals::game.pet().setStart(newCoordinate);
  Globals::game.pet().redraw(Globals::game.screen());
}

void movePet(
    EspGL::Hitbox newHitbox,
    const std::vector<EspGL::Hitbox>& hitboxes) {
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
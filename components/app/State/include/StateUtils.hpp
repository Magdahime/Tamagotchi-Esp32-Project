#pragma once
#include "Drawable.hpp"
#include "Globals.hpp"
namespace tamagotchi {
namespace App {
namespace State {

namespace consts {
constexpr int USER_INPUT_WAIT_TIME = 100;
constexpr auto ICON_COLOUR = EspGL::colours::WHITE;
constexpr std::pair<int, int> START_ICONS = {0, 0};
constexpr int ICONS_SPACING = 5;  // in pixels
constexpr int ICONS_SCALE = 5;
constexpr int STEP_X = 20;  // in pixels
constexpr int STEP_Y = 20;  // in pixels
constexpr int THRESHOLD = 10;
}  // namespace consts

namespace StateUtils {

void movePet(const std::vector<EspGL::Hitbox>& hitboxes);
void movePet(EspGL::Hitbox newHitbox,
             const std::vector<EspGL::Hitbox>& hitboxes);
bool checkCollision(EspGL::Hitbox newHitbox,
                    const std::vector<EspGL::Hitbox>& hitboxes);

}  // namespace StateUtils
}  // namespace State
}  // namespace App
}  // namespace tamagotchi
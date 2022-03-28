#include "Drawable.hpp"
#include "Globals.hpp"
namespace tamagotchi {
namespace App {
namespace State {

namespace consts {
constexpr auto ICON_COLOUR = EspGL::colours::WHITE;
constexpr std::pair<int, int> START_ICONS = {0, 0};
constexpr int ICONS_SPACING = 5;  // in pixels
constexpr int ICONS_SCALE = 5;
constexpr int STEP_X = 20;  // in pixels
constexpr int STEP_Y = 20;  // in pixels
constexpr int THRESHHOLD = 10;
}  // namespace consts

namespace StateUtils {

void movePet(
    const std::vector<EspGL::EspGLHitbox>& hitboxes);
void movePet(
    EspGL::EspGLHitbox newHitbox,
    const std::vector<EspGL::EspGLHitbox>& hitboxes);
bool checkCollision(
    EspGL::EspGLHitbox newHitbox,
    const std::vector<EspGL::EspGLHitbox>& hitboxes);

}  // namespace StateUtils
}  // namespace State
}  // namespace App
}  // namespace tamagotchi
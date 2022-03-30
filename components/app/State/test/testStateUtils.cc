
#include <stdio.h>

#include <cstring>
#include <string>

#include "EspGLUtils.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "StateUtils.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(StateUtilsTests);

TEST_SETUP(StateUtilsTests) {}

TEST_TEAR_DOWN(StateUtilsTests) {}

// TEST(StateUtilsTests, MovePetTest) {
//   int pets = 10;
//   std::vector<tamagotchi::EspGL::Hitbox> hitboxes;
//   tamagotchi::EspGL::Vect2 start{239, 0};
//   for (auto i = 0; i < pets; ++i) {
//     tamagotchi::App::State::StateUtils::movePet(
//         tamagotchi::EspGL::Vect2(
//             start.x_,
//             start.y_ + tamagotchi::App::Globals::game.screen().height() / pets),
//         hitboxes);
//   }
// }

TEST_GROUP_RUNNER(StateUtilsTests) {
  //RUN_TEST_CASE(StateUtilsTests, MovePetTest)
}
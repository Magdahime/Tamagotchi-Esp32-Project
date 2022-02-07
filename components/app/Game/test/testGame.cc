#include "Game.hpp"
#include "Globals.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(GameTests);

TEST_SETUP(GameTests) {}

TEST_TEAR_DOWN(GameTests) {}

TEST(GameTests, CreateGame) {}

TEST_GROUP_RUNNER(GameTests) { RUN_TEST_CASE(GameTests, CreateGame) }
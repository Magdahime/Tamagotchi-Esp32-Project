#include <iostream>

#include "EspGL.hpp"
#include "Globals.hpp"
#include "Graphics/Font/EspGLFont.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP(EspGLFontLoaderTests);

TEST_SETUP(EspGLFontLoaderTests) {}

TEST_TEAR_DOWN(EspGLFontLoaderTests) { tamagotchi::EspGL::delay(1000); }

TEST(EspGLFontLoaderTests, LoadBitmapTest) {
  tamagotchi::EspGL::FontLoader fLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  tamagotchi::EspGL::Font font = fLoader.load();

  TEST_ASSERT_EQUAL_INT(0, font.null().empty());
  TEST_ASSERT_EQUAL_INT(69, font.size());
}

TEST_GROUP_RUNNER(EspGLFontLoaderTests) {
  RUN_TEST_CASE(EspGLFontLoaderTests, LoadBitmapTest)
}
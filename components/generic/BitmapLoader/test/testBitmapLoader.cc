#include <iostream>

#include "EspGL.hpp"
#include "Globals.hpp"
#include "Graphics/Font/EspGLFont.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(BitmapLoaderTests);

TEST_SETUP(BitmapLoaderTests) {}

TEST_TEAR_DOWN(BitmapLoaderTests) { tamagotchi::EspGL::delay(1000); }

TEST(BitmapLoaderTests, ParseMagicNumberTest) {
  tamagotchi::BitmapLoader::BitmapLoader bitmapLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  std::string magicNumber = bitmapLoader.parseMagicNumber();
  TEST_ASSERT_EQUAL_STRING("P1", magicNumber.c_str());
  ESP_LOGI(tamagotchi::EspGL::TAG_, "Read from fullFont.pbm: %s",
           magicNumber.c_str());
}

TEST(BitmapLoaderTests, ParseDimensionsTest) {
  tamagotchi::BitmapLoader::BitmapLoader bitmapLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  bitmapLoader.parseMagicNumber();
  auto dimensions = bitmapLoader.parseDimensions();
  TEST_ASSERT_EQUAL_INT(dimensions.first, 6);
  TEST_ASSERT_EQUAL_INT(dimensions.second, 8);
}

TEST(BitmapLoaderTests, ParseLettersTest) {
  tamagotchi::BitmapLoader::BitmapLoader bitmapLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  bitmapLoader.parseMagicNumber();
  bitmapLoader.parseDimensions();
  auto bitmapNumber = bitmapLoader.parseBitmapNumber();
  auto letters = bitmapLoader.parseSymbols(bitmapNumber);
  TEST_ASSERT_EQUAL_INT(70, letters.size());
  TEST_ASSERT_EQUAL_STRING(letters.front().c_str(), "0");
  TEST_ASSERT_EQUAL_STRING(letters.back().c_str(), "null");
}

TEST(BitmapLoaderTests, ParseBitmapTest) {
  tamagotchi::BitmapLoader::BitmapLoader bitmapLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  std::map<std::string, std::vector<bool>> fontMap;
  bitmapLoader.parseMagicNumber();
  auto dimensions = bitmapLoader.parseDimensions();
  auto bitmapNumber = bitmapLoader.parseBitmapNumber();
  auto letters = bitmapLoader.parseSymbols(bitmapNumber);
  for (auto& letter : letters) {
    fontMap.insert(std::make_pair(
        letter, bitmapLoader.parseBitmap(dimensions.first, dimensions.second)));
  }
  TEST_ASSERT_EQUAL_INT(70, fontMap.size());
  for (auto& [key, value] : fontMap) {
    TEST_ASSERT_EQUAL_INT(value.size(), 48);
  }
}

TEST_GROUP_RUNNER(BitmapLoaderTests) {
  RUN_TEST_CASE(BitmapLoaderTests, ParseMagicNumberTest)
  RUN_TEST_CASE(BitmapLoaderTests, ParseDimensionsTest)
  RUN_TEST_CASE(BitmapLoaderTests, ParseLettersTest)
  RUN_TEST_CASE(BitmapLoaderTests, ParseBitmapTest)
}
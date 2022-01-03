#include <iostream>

#include "EspGL.hpp"
#include "Graphics/Font/EspGLFont.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "SPIFFSDriver.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(EspGLFontLoaderTests);

TEST_SETUP(EspGLFontLoaderTests) {}

TEST_TEAR_DOWN(EspGLFontLoaderTests) { tamagotchi::EspGL::delay(1000); }

TEST(EspGLFontLoaderTests, ParseMagicNumberTest) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver =
      tamagotchi::SPIFFS::SPIFFSDriver();
  tamagotchi::EspGL::FontLoader fLoader(
      spiffsDriver.getFileDescriptor("fullFont.pbm"));
  std::string magicNumber = fLoader.parseMagicNumber();
  TEST_ASSERT_EQUAL_STRING("P1", magicNumber.c_str());
  ESP_LOGI(tamagotchi::EspGL::TAG_, "Read from fullFont.pbm: %s",
           magicNumber.c_str());
}

TEST(EspGLFontLoaderTests, ParseDimensionsTest) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver =
      tamagotchi::SPIFFS::SPIFFSDriver();
  tamagotchi::EspGL::FontLoader fLoader(
      spiffsDriver.getFileDescriptor("fullFont.pbm"));
  fLoader.parseMagicNumber();
  auto dimensions = fLoader.parseDimensions();
  TEST_ASSERT_EQUAL_INT(dimensions.first, 6);
  TEST_ASSERT_EQUAL_INT(dimensions.second, 8);
}

TEST(EspGLFontLoaderTests, ParseLettersTest) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver =
      tamagotchi::SPIFFS::SPIFFSDriver();
  tamagotchi::EspGL::FontLoader fLoader(
      spiffsDriver.getFileDescriptor("fullFont.pbm"));
  fLoader.parseMagicNumber();
  fLoader.parseDimensions();
  auto letters = fLoader.parseLetters();
  TEST_ASSERT_EQUAL_INT(37, letters.size());
  TEST_ASSERT_EQUAL_STRING(letters.front().c_str(), "0");
  TEST_ASSERT_EQUAL_STRING(letters.back().c_str(), "null");
}

TEST(EspGLFontLoaderTests, ParseBitmapTest) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver =
      tamagotchi::SPIFFS::SPIFFSDriver();
  tamagotchi::EspGL::FontLoader fLoader(
      spiffsDriver.getFileDescriptor("fullFont.pbm"));
  std::map<std::string, tamagotchi::EspGL::Bitmap> fontMap;
  fLoader.parseMagicNumber();
  auto dimensions = fLoader.parseDimensions();
  auto letters = fLoader.parseLetters();
  for (auto& letter : letters) {
    fontMap.insert(std::make_pair(
        letter, fLoader.parseBitmap(dimensions.first, dimensions.second)));
  }
  TEST_ASSERT_EQUAL_INT(37, fontMap.size());
  for (auto& [key, value] : fontMap) {
    TEST_ASSERT_EQUAL_INT(value.bitmap().size(), 48);
  }
}

TEST(EspGLFontLoaderTests, LoadBitmapTest) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver =
      tamagotchi::SPIFFS::SPIFFSDriver();
  tamagotchi::EspGL::FontLoader fLoader(
      spiffsDriver.getFileDescriptor("fullFont.pbm"));
  tamagotchi::EspGL::Font font = fLoader.load();

  TEST_ASSERT_EQUAL_INT(0, font.null().empty());
  TEST_ASSERT_EQUAL_INT(37, font.size());
}

TEST_GROUP_RUNNER(EspGLFontLoaderTests) {
  RUN_TEST_CASE(EspGLFontLoaderTests, ParseMagicNumberTest)
  RUN_TEST_CASE(EspGLFontLoaderTests, ParseDimensionsTest)
  RUN_TEST_CASE(EspGLFontLoaderTests, ParseLettersTest)
  RUN_TEST_CASE(EspGLFontLoaderTests, ParseBitmapTest)
  RUN_TEST_CASE(EspGLFontLoaderTests, LoadBitmapTest)
}
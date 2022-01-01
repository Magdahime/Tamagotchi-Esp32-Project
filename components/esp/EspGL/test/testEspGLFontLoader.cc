#include "EspGL.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "SPIFFSDriver.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(EspGLFontLoaderTests);

TEST_SETUP(EspGLFontLoaderTests) {}

TEST_TEAR_DOWN(EspGLFontLoaderTests) {}

TEST(EspGLFontLoaderTests, ParseMagicNumberTest) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver;
  tamagotchi::EspGL::FontLoader fLoader(
      spiffsDriver.getFileDescriptor("fullFont.pbm"));
  std::string magicNumber = fLoader.parseMagicNumber();
  TEST_ASSERT_EQUAL_STRING("P1", magicNumber.c_str());
  ESP_LOGI(tamagotchi::EspGL::TAG_, "Read from fullFont.pbm: %s",
           magicNumber.c_str());
}

TEST_GROUP_RUNNER(EspGLFontLoaderTests) {
  RUN_TEST_CASE(EspGLFontLoaderTests, ParseMagicNumberTest)
}
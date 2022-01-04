#include "EspGLScreen.hpp"
#include "EspGLUtils.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "Graphics/Text/EspGLText.hpp"
#include "SPIFFSDriver.hpp"
#include "ST7789Driver.hpp"
#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::ST7789;
using namespace tamagotchi::EspGL;
using namespace tamagotchi::SPIFFS;

constexpr int SHORT_PAUSE = 1000;

extern tamagotchi::EspGL::Screen<uint16_t> screen;
extern Point center;

TEST_GROUP(EspGLTextTests);

TEST_SETUP(EspGLTextTests) {
  tamagotchi::EspGL::delay(SHORT_PAUSE);
  screen.fill(Colour<uint16_t>(colours::BLACK));
  tamagotchi::EspGL::delay(SHORT_PAUSE);
}

TEST_TEAR_DOWN(EspGLTextTests) {}

TEST(EspGLTextTests, PrintingTextTest) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver =
      tamagotchi::SPIFFS::SPIFFSDriver();
  tamagotchi::EspGL::FontLoader fLoader(
      spiffsDriver.getFileDescriptor("fullFont.pbm"));
  Font font = fLoader.load();
  Text<uint16_t> text("siurek", font, Colour<uint16_t>(colours::GREEN));
  text.draw(screen, {0, 0});
}

TEST_GROUP_RUNNER(EspGLTextTests) {
  RUN_TEST_CASE(EspGLTextTests, PrintingTextTest)
}
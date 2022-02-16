#include "EspGLScreen.hpp"
#include "EspGLUtils.hpp"
#include "Globals.hpp"
#include "Graphics/Font/EspGLFontLoader.hpp"
#include "Graphics/Text/EspGLText.hpp"
#include "ST7789Driver.hpp"
#include "unity.h"
#include "unity_fixture.h"

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
  tamagotchi::EspGL::FontLoader fLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  Font font = fLoader.load();
  Text<uint16_t> text("Hello World", font, Colour<uint16_t>(colours::GREEN));
  text.draw(screen, {0, 0});
}

TEST(EspGLTextTests, PrintingAlphabetTest) {
  tamagotchi::EspGL::FontLoader fLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  Font font = fLoader.load();
  Text<uint16_t> text(
      "0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v w x y z "
      "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z !?.,:;%#",
      font, Colour<uint16_t>(colours::RED));
  text.draw(screen, {0, 0});
}

TEST(EspGLTextTests, PrintingLongTextTest) {
  tamagotchi::EspGL::FontLoader fLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  Font font = fLoader.load();
  Text<uint16_t> text(
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer quis "
      "felis tincidunt, auctor nisl nec, iaculis velit. Morbi imperdiet "
      "eleifend auctor. Curabitur a ipsum sed ante pretium sollicitudin tempor "
      "ac turpis. Nulla sed fringilla quam. In eu odio vulputate, tristique "
      "quam accumsan, hendrerit ante. Proin accumsan ligula sit amet libero "
      "consequat tincidunt. Vestibulum pharetra cursus vulputate. Duis ac nisi "
      "ornare, suscipit libero non, varius felis. Cras laoreet luctus quam, et "
      "tristique purus venenatis euismod. Duis in feugiat quam, ut mattis mi. "
      "Phasellus at semper nulla. In hac habitasse platea dictumst. "
      "Suspendisse sem lectus, finibus imperdiet aliquet ut, pellentesque a "
      "orci. Etiam luctus ultrices mattis.",
      font, Colour<uint16_t>(colours::RED));
  text.draw(screen, {0, 0});
}

TEST(EspGLTextTests, DebuggingTest) {
  tamagotchi::EspGL::FontLoader fLoader(
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("fullFont.pbm"));
  Font font = fLoader.load();
  Text<uint16_t> text(
      "u u v v w w x x y y z z . . , , : : ; ; ! ! % % ? ? # #",
      font, Colour<uint16_t>(colours::RED));
  text.draw(screen, {0, 0});
}

TEST_GROUP_RUNNER(EspGLTextTests) {
  RUN_TEST_CASE(EspGLTextTests, PrintingTextTest)
  RUN_TEST_CASE(EspGLTextTests, PrintingAlphabetTest)
  RUN_TEST_CASE(EspGLTextTests, PrintingLongTextTest)
  RUN_TEST_CASE(EspGLTextTests, DebuggingTest)
}
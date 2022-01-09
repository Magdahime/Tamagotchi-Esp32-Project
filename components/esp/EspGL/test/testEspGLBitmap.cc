#include <stdexcept>

#include "Graphics/EspGLBitmap.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(EspGLBitmapTests);

TEST_SETUP(EspGLBitmapTests) {}

TEST_TEAR_DOWN(EspGLBitmapTests) {}

TEST(EspGLBitmapTests, OverlayEmptyBitmap) {
  std::vector<bool> bitmap{false, false, true, true};
  tamagotchi::EspGL::Bitmap bitmap1;
  tamagotchi::EspGL::Bitmap bitmap2(2, 2, bitmap);
  tamagotchi::EspGL::Point point{0, 0};
  try {
    bitmap1.overlay(bitmap2, point);
  }

  catch (const std::runtime_error& e) {
    TEST_ASSERT_EQUAL_STRING("Primary bitmap in overlay() method is empty!",
                             e.what());
    TEST_PASS();
  }

  TEST_FAIL_MESSAGE("Expected throw()");
}

TEST(EspGLBitmapTests, OverlayTwoBitmaps) {
  tamagotchi::EspGL::Bitmap bitmap1(2, 2,
                                    std::vector<bool>{true, true, true, true});
  tamagotchi::EspGL::Bitmap bitmap2(
      2, 2, std::vector<bool>{false, false, false, false});
  tamagotchi::EspGL::Point point{0, 0};
  bitmap1.overlay(bitmap2, point);
  TEST_ASSERT_EQUAL_INT(0, bitmap1.at(0));
  TEST_ASSERT_EQUAL_INT(0, bitmap1.at(1));
  TEST_ASSERT_EQUAL_INT(0, bitmap1.at(2));
  TEST_ASSERT_EQUAL_INT(0, bitmap1.at(3));
}

TEST_GROUP_RUNNER(EspGLBitmapTests) {
  RUN_TEST_CASE(EspGLBitmapTests, OverlayEmptyBitmap)
  RUN_TEST_CASE(EspGLBitmapTests, OverlayTwoBitmaps)
}
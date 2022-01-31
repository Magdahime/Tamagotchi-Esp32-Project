#include <stdio.h>

#include <cstring>
#include <string>
#include "Globals.hpp"
#include "SPIFFSDriver.hpp"
#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::SPIFFS;

TEST_GROUP(SPIFFSDriverTests);

TEST_SETUP(SPIFFSDriverTests) {}

TEST_TEAR_DOWN(SPIFFSDriverTests) {}

TEST(SPIFFSDriverTests, ReadingFromFileTest) {
  auto fileHandler = tamagotchi::App::Globals::spiffsDriver.getFileDescriptor(
      "hello_world.txt");
  std::string line;
  std::getline(fileHandler, line);
  TEST_ASSERT_EQUAL_STRING("Hello World!", line.c_str());
  ESP_LOGI(TAG_, "Read from hello.txt: %s", line.c_str());
}

TEST(SPIFFSDriverTests, CreatingNewFile) {
  auto fileHandler =
      tamagotchi::App::Globals::spiffsDriver.createNewFile("test.txt");
  fileHandler << "test\n";
}

TEST(SPIFFSDriverTests, ReadingFromNewFile) {
  auto fileHandler =
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("test.txt");
  std::string line;
  std::getline(fileHandler, line);
  TEST_ASSERT_EQUAL_STRING("test", line.c_str());
}

TEST(SPIFFSDriverTests, DeletingFile) {
  TEST_ASSERT_TRUE(
      tamagotchi::App::Globals::spiffsDriver.deleteFile("test.txt"));
}

TEST_GROUP_RUNNER(SPIFFSDriverTests) {
  RUN_TEST_CASE(SPIFFSDriverTests, ReadingFromFileTest)
  RUN_TEST_CASE(SPIFFSDriverTests, CreatingNewFile)
  RUN_TEST_CASE(SPIFFSDriverTests, ReadingFromNewFile);
  RUN_TEST_CASE(SPIFFSDriverTests, DeletingFile);
}
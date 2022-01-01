#include <cstring>
#include <string>

#include "SPIFFSDriver.hpp"
#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::SPIFFS;

TEST_GROUP(SPIFFSDriverTests);

TEST_SETUP(SPIFFSDriverTests) {}

TEST_TEAR_DOWN(SPIFFSDriverTests) {}

TEST(SPIFFSDriverTests, MountingSPIFFSTest) { SPIFFSDriver(); }

TEST(SPIFFSDriverTests, ReadingFromFileTest) {
  SPIFFSDriver spiffsDriver = SPIFFSDriver();
  auto fileHandler = spiffsDriver.getFileDescriptor("hello_world.txt");
  std::string line;
  std::getline(fileHandler, line);
  TEST_ASSERT_EQUAL_STRING("Hello World!", line.c_str());
  ESP_LOGI(TAG_, "Read from hello.txt: %s", line.c_str());
}

TEST_GROUP_RUNNER(SPIFFSDriverTests) {
  RUN_TEST_CASE(SPIFFSDriverTests, MountingSPIFFSTest)
  RUN_TEST_CASE(SPIFFSDriverTests, ReadingFromFileTest)
}
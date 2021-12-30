#include "SPIFFSDriver.hpp"
#include <cstring>
#include "unity.h"
#include "unity_fixture.h"

using namespace tamagotchi::SPIFFS;

TEST_GROUP(SPIFFSDriverTests);

TEST_SETUP(SPIFFSDriverTests) {}

TEST_TEAR_DOWN(SPIFFSDriverTests) {}

TEST(SPIFFSDriverTests, MountingSPIFFSTest) { SPIFFSDriver(); }

TEST(SPIFFSDriverTests, ReadingFromFileTest) {
  SPIFFSDriver spiffsDriver = SPIFFSDriver();
  auto fileHandler = spiffsDriver.getFileDescriptor("hello_world.txt", "r");
  char buf[64];
  memset(buf, 0, sizeof(buf));
  fread(buf, 1, sizeof(buf), fileHandler);
  fclose(fileHandler);
  ESP_LOGI(TAG_, "Read from hello.txt: %s", buf);
}

TEST_GROUP_RUNNER(SPIFFSDriverTests) {
  RUN_TEST_CASE(SPIFFSDriverTests, MountingSPIFFSTest)
  RUN_TEST_CASE(SPIFFSDriverTests, ReadingFromFileTest)
}
#include <stdio.h>

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

TEST(SPIFFSDriverTests, CreatingNewFile) {
  SPIFFSDriver spiffsDriver = SPIFFSDriver();
  auto fileHandler = spiffsDriver.createNewFile("test.txt");
  fileHandler << "test\n";
}

TEST(SPIFFSDriverTests, ReadingFromNewFile) {
  SPIFFSDriver spiffsDriver = SPIFFSDriver();
  auto fileHandler = spiffsDriver.getFileDescriptor("test.txt");
  std::string line;
  std::getline(fileHandler, line);
  TEST_ASSERT_EQUAL_STRING("test", line.c_str());
}

TEST(SPIFFSDriverTests, DeletingFile) {
  SPIFFSDriver spiffsDriver = SPIFFSDriver();
  TEST_ASSERT_TRUE(spiffsDriver.deleteFile("test.txt"));
}

TEST_GROUP_RUNNER(SPIFFSDriverTests) {
  RUN_TEST_CASE(SPIFFSDriverTests, MountingSPIFFSTest)
  RUN_TEST_CASE(SPIFFSDriverTests, ReadingFromFileTest)
  RUN_TEST_CASE(SPIFFSDriverTests, CreatingNewFile)
  RUN_TEST_CASE(SPIFFSDriverTests, ReadingFromNewFile);
  RUN_TEST_CASE(SPIFFSDriverTests, DeletingFile);
}
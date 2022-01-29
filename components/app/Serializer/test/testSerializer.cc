#include "SPIFFSDriver.hpp"
#include "Serializer.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(SerializerTests);

TEST_SETUP(SerializerTests) {}

TEST_TEAR_DOWN(SerializerTests) {}

TEST(SerializerTests, SerializeString) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver;
  std::string testString = "hello world!";
  auto serializeFileHandle = spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, testString);
  serializeFileHandle.close();
  auto deserializeFileHandle = spiffsDriver.getFileDescriptor("test.txt");
  std::string deserializedString;
  serializer.deserialize(deserializeFileHandle, deserializedString);
  TEST_ASSERT_EQUAL_STRING(testString.c_str(), deserializedString.c_str());
}

TEST_GROUP_RUNNER(SerializerTests) {
  RUN_TEST_CASE(SerializerTests, SerializeString)
}
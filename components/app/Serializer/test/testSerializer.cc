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

TEST(SerializerTests, SerializeArithmeticType) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver;
  int test = 12345;
  auto serializeFileHandle = spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, test);
  serializeFileHandle.close();
  auto deserializeFileHandle = spiffsDriver.getFileDescriptor("test.txt");
  int deserializedTest;
  serializer.deserialize(deserializeFileHandle, deserializedTest);
  TEST_ASSERT_EQUAL_INT(test, deserializedTest);
}

TEST(SerializerTests, SerializeVectorOfArithmeticType) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver;
  std::vector<int> test{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto serializeFileHandle = spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, test);
  serializeFileHandle.close();
  auto deserializeFileHandle = spiffsDriver.getFileDescriptor("test.txt");
  std::vector<int> deserializedTest;
  serializer.deserialize(deserializeFileHandle, deserializedTest);
  TEST_ASSERT_EQUAL_INT(test.size(), deserializedTest.size());
  for (int i = 0; i < deserializedTest.size(); i++) {
    TEST_ASSERT_EQUAL_INT(test[i], deserializedTest[i]);
  }
}

TEST(SerializerTests, SerializeVectorOfBool) {
  tamagotchi::SPIFFS::SPIFFSDriver spiffsDriver;
  std::vector<bool> test{true,  false, true,  false, true,
                         false, true,  false, false};
  auto serializeFileHandle = spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, test);
  serializeFileHandle.close();
  auto deserializeFileHandle = spiffsDriver.getFileDescriptor("test.txt");
  std::vector<bool> deserializedTest;
  serializer.deserialize(deserializeFileHandle, deserializedTest);
  TEST_ASSERT_EQUAL_INT(test.size(), deserializedTest.size());
  for (int i = 0; i < deserializedTest.size(); i++) {
    TEST_ASSERT_EQUAL_INT(test[i], deserializedTest[i]);
  }
}

TEST_GROUP_RUNNER(SerializerTests) {
  RUN_TEST_CASE(SerializerTests, SerializeString)
  RUN_TEST_CASE(SerializerTests, SerializeArithmeticType)
  RUN_TEST_CASE(SerializerTests, SerializeVectorOfArithmeticType)
  RUN_TEST_CASE(SerializerTests, SerializeVectorOfBool)
}
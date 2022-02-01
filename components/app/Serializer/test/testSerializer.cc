#include <stdint.h>

#include "Globals.hpp"
#include "PetGenerator.hpp"
#include "Serializer.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(SerializerTests);

TEST_SETUP(SerializerTests) {}

TEST_TEAR_DOWN(SerializerTests) {}

TEST(SerializerTests, SerializeString) {
  std::string testString = "hello world!";
  auto serializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, testString);
  serializeFileHandle.close();
  auto deserializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("test.txt");
  std::string deserializedString;
  serializer.deserialize(deserializeFileHandle, deserializedString);
  TEST_ASSERT_EQUAL_STRING(testString.c_str(), deserializedString.c_str());
}

TEST(SerializerTests, SerializeArithmeticType) {
  int test = 12345;
  auto serializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, test);
  serializeFileHandle.close();
  auto deserializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("test.txt");
  int deserializedTest;
  serializer.deserialize(deserializeFileHandle, deserializedTest);
  TEST_ASSERT_EQUAL_INT(test, deserializedTest);
}

TEST(SerializerTests, SerializeVectorOfArithmeticType) {
  std::vector<int> test{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto serializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, test);
  serializeFileHandle.close();
  auto deserializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("test.txt");
  std::vector<int> deserializedTest;
  serializer.deserialize(deserializeFileHandle, deserializedTest);
  TEST_ASSERT_EQUAL_INT(test.size(), deserializedTest.size());
  for (int i = 0; i < deserializedTest.size(); i++) {
    TEST_ASSERT_EQUAL_INT(test[i], deserializedTest[i]);
  }
}

TEST(SerializerTests, SerializeVectorOfBool) {
  std::vector<bool> test{true,  false, true,  false, true,
                         false, true,  false, false};
  auto serializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.createNewFile("test.txt");
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(serializeFileHandle, test);
  serializeFileHandle.close();
  auto deserializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("test.txt");
  std::vector<bool> deserializedTest;
  serializer.deserialize(deserializeFileHandle, deserializedTest);
  TEST_ASSERT_EQUAL_INT(test.size(), deserializedTest.size());
  for (int i = 0; i < deserializedTest.size(); i++) {
    TEST_ASSERT_EQUAL_INT(test[i], deserializedTest[i]);
  }
}

TEST(SerializerTests, SerializePet) {
  tamagotchi::App::Pet::Pet<uint16_t> pet;
  tamagotchi::App::Pet::PetGenerator<uint16_t> petGen("petComponents.pbm");
  pet = petGen.generate();
  tamagotchi::App::Serializer::Serializer serializer;
  pet.accept(serializer);
  tamagotchi::App::Pet::Pet<uint16_t> deserializedPet;
  auto deserializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor("pet.ser");
  serializer.deserialize(deserializeFileHandle, deserializedPet);
  TEST_ASSERT_EQUAL_STRING(pet.name().c_str(), deserializedPet.name().c_str());
  TEST_ASSERT_EQUAL_INT(pet.needs().size(), deserializedPet.needs().size());
  for (int i = 0; i < deserializedPet.needs().size(); i++) {
    TEST_ASSERT_EQUAL_INT(pet.needs()[i], deserializedPet.needs()[i]);
  }
  TEST_ASSERT_EQUAL_INT(pet.colour().value(), deserializedPet.colour().value());

  TEST_ASSERT_EQUAL_STRING(pet.body().first.c_str(),
                           deserializedPet.body().first.c_str());

  TEST_ASSERT_EQUAL_INT(pet.body().second.sizeX(),
                        deserializedPet.body().second.sizeX());
  TEST_ASSERT_EQUAL_INT(pet.body().second.sizeY(),
                        deserializedPet.body().second.sizeY());
  TEST_ASSERT_EQUAL_INT(pet.body().second.bitmap().size(),
                        deserializedPet.body().second.bitmap().size());
  for (int i = 0; i < deserializedPet.body().second.bitmap().size(); i++) {
    TEST_ASSERT_EQUAL_INT(pet.body().second.bitmap()[i],
                          deserializedPet.body().second.bitmap()[i]);
  }

  TEST_ASSERT_EQUAL_STRING(pet.eyes().first.c_str(),
                           deserializedPet.eyes().first.c_str());

  TEST_ASSERT_EQUAL_INT(pet.eyes().second.sizeX(),
                        deserializedPet.eyes().second.sizeX());
  TEST_ASSERT_EQUAL_INT(pet.eyes().second.sizeY(),
                        deserializedPet.eyes().second.sizeY());
  TEST_ASSERT_EQUAL_INT(pet.eyes().second.bitmap().size(),
                        deserializedPet.eyes().second.bitmap().size());
  for (int i = 0; i < deserializedPet.eyes().second.bitmap().size(); i++) {
    TEST_ASSERT_EQUAL_INT(pet.eyes().second.bitmap()[i],
                          deserializedPet.eyes().second.bitmap()[i]);
  }

  TEST_ASSERT_EQUAL_STRING(pet.face().first.c_str(),
                           deserializedPet.face().first.c_str());
  TEST_ASSERT_EQUAL_INT(pet.face().second.sizeX(),
                        deserializedPet.face().second.sizeX());
  TEST_ASSERT_EQUAL_INT(pet.face().second.sizeY(),
                        deserializedPet.face().second.sizeY());
  TEST_ASSERT_EQUAL_INT(pet.face().second.bitmap().size(),
                        deserializedPet.face().second.bitmap().size());
  for (int i = 0; i < deserializedPet.face().second.bitmap().size(); i++) {
    TEST_ASSERT_EQUAL_INT(pet.face().second.bitmap()[i],
                          deserializedPet.face().second.bitmap()[i]);
  }
}

TEST_GROUP_RUNNER(SerializerTests) {
  RUN_TEST_CASE(SerializerTests, SerializeString)
  RUN_TEST_CASE(SerializerTests, SerializeArithmeticType)
  RUN_TEST_CASE(SerializerTests, SerializeVectorOfArithmeticType)
  RUN_TEST_CASE(SerializerTests, SerializeVectorOfBool)
  RUN_TEST_CASE(SerializerTests, SerializePet);
}

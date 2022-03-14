#include <stdint.h>

#include <iostream>

#include "DrawablePet.hpp"
#include "EspGLUtils.hpp"
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
  serializer.serialize(&pet);
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

TEST(SerializerTests, SerializeDrawablePet) {
  tamagotchi::App::Pet::Pet<uint16_t> dummyPet;
  tamagotchi::App::Pet::PetGenerator<uint16_t> petGen("petComponents.pbm");
  dummyPet = petGen.generate();
  tamagotchi::App::Pet::DrawablePet<uint16_t> drawablePet(
      dummyPet, tamagotchi::EspGL::Point(0, 0), 5);
  tamagotchi::App::Serializer::Serializer serializer;
  serializer.serialize(&drawablePet);
  tamagotchi::App::Pet::DrawablePet<uint16_t> deserializedDrawablePet;
  auto deserializeFileHandle =
      tamagotchi::App::Globals::spiffsDriver.getFileDescriptor(
          "DrawablePet.ser");
  serializer.deserialize(deserializeFileHandle, deserializedDrawablePet);

  TEST_ASSERT_EQUAL_STRING(drawablePet.name().c_str(),
                           deserializedDrawablePet.name().c_str());
  TEST_ASSERT_EQUAL_INT(drawablePet.needs().size(),
                        deserializedDrawablePet.needs().size());
  for (int i = 0; i < deserializedDrawablePet.needs().size(); i++) {
    TEST_ASSERT_EQUAL_INT(drawablePet.needs()[i],
                          deserializedDrawablePet.needs()[i]);
  }
  TEST_ASSERT_EQUAL_INT(drawablePet.colour().value(),
                        deserializedDrawablePet.colour().value());

  TEST_ASSERT_EQUAL_STRING(drawablePet.body().first.c_str(),
                           deserializedDrawablePet.body().first.c_str());

  TEST_ASSERT_EQUAL_INT(drawablePet.body().second.sizeX(),
                        deserializedDrawablePet.body().second.sizeX());
  TEST_ASSERT_EQUAL_INT(drawablePet.body().second.sizeY(),
                        deserializedDrawablePet.body().second.sizeY());
  TEST_ASSERT_EQUAL_INT(drawablePet.body().second.bitmap().size(),
                        deserializedDrawablePet.body().second.bitmap().size());
  for (int i = 0; i < deserializedDrawablePet.body().second.bitmap().size();
       i++) {
    TEST_ASSERT_EQUAL_INT(drawablePet.body().second.bitmap()[i],
                          deserializedDrawablePet.body().second.bitmap()[i]);
  }

  TEST_ASSERT_EQUAL_STRING(drawablePet.eyes().first.c_str(),
                           deserializedDrawablePet.eyes().first.c_str());

  TEST_ASSERT_EQUAL_INT(drawablePet.eyes().second.sizeX(),
                        deserializedDrawablePet.eyes().second.sizeX());
  TEST_ASSERT_EQUAL_INT(drawablePet.eyes().second.sizeY(),
                        deserializedDrawablePet.eyes().second.sizeY());
  TEST_ASSERT_EQUAL_INT(drawablePet.eyes().second.bitmap().size(),
                        deserializedDrawablePet.eyes().second.bitmap().size());
  for (int i = 0; i < deserializedDrawablePet.eyes().second.bitmap().size();
       i++) {
    TEST_ASSERT_EQUAL_INT(drawablePet.eyes().second.bitmap()[i],
                          deserializedDrawablePet.eyes().second.bitmap()[i]);
  }

  TEST_ASSERT_EQUAL_STRING(drawablePet.face().first.c_str(),
                           deserializedDrawablePet.face().first.c_str());
  TEST_ASSERT_EQUAL_INT(drawablePet.face().second.sizeX(),
                        deserializedDrawablePet.face().second.sizeX());
  TEST_ASSERT_EQUAL_INT(drawablePet.face().second.sizeY(),
                        deserializedDrawablePet.face().second.sizeY());
  TEST_ASSERT_EQUAL_INT(drawablePet.face().second.bitmap().size(),
                        deserializedDrawablePet.face().second.bitmap().size());
  for (int i = 0; i < deserializedDrawablePet.face().second.bitmap().size();
       i++) {
    TEST_ASSERT_EQUAL_INT(drawablePet.face().second.bitmap()[i],
                          deserializedDrawablePet.face().second.bitmap()[i]);
  }

  TEST_ASSERT_EQUAL_INT(drawablePet.scale(), deserializedDrawablePet.scale());
  TEST_ASSERT_EQUAL_INT(drawablePet.start().x_,
                        deserializedDrawablePet.start().x_);
  TEST_ASSERT_EQUAL_INT(drawablePet.start().y_,
                        deserializedDrawablePet.start().y_);
}

TEST_GROUP_RUNNER(SerializerTests) {
  RUN_TEST_CASE(SerializerTests, SerializeString)
  RUN_TEST_CASE(SerializerTests, SerializeArithmeticType)
  RUN_TEST_CASE(SerializerTests, SerializeVectorOfArithmeticType)
  RUN_TEST_CASE(SerializerTests, SerializeVectorOfBool)
  RUN_TEST_CASE(SerializerTests, SerializePet);
  RUN_TEST_CASE(SerializerTests, SerializeDrawablePet);
}

#include "PetGenerator.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(PetGeneratorTests);

TEST_SETUP(PetGeneratorTests) {}

TEST_TEAR_DOWN(PetGeneratorTests) {}

TEST(PetGeneratorTests, LoadDataTest) {
  tamagotchi::App::Pet::PetGenerator<uint16_t> petGenerator(
      "petComponents.pbm");
  TEST_ASSERT_EQUAL_INT(6, petGenerator.bodies().size());
  TEST_ASSERT_EQUAL_INT(6, petGenerator.eyes().size());
  TEST_ASSERT_EQUAL_INT(6, petGenerator.faces().size());

  TEST_ASSERT_EQUAL_INT(20, petGenerator.bodies().at("antennaeEars").sizeX());
  TEST_ASSERT_EQUAL_INT(20, petGenerator.bodies().at("antennaeEars").sizeY());

  TEST_ASSERT_EQUAL_INT(9, petGenerator.eyes().at("cuteEyes").sizeX());
  TEST_ASSERT_EQUAL_INT(3, petGenerator.eyes().at("cuteEyes").sizeY());

  TEST_ASSERT_EQUAL_INT(9, petGenerator.faces().at("weirdFace").sizeX());
  TEST_ASSERT_EQUAL_INT(3, petGenerator.faces().at("weirdFace").sizeY());
}

TEST_GROUP_RUNNER(PetGeneratorTests) {
  RUN_TEST_CASE(PetGeneratorTests, LoadDataTest)
}
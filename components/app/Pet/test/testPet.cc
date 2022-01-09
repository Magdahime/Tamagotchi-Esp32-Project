#include "Pet.hpp"
#include "PetGenerator.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(PetTests);

TEST_SETUP(PetTests) {}

TEST_TEAR_DOWN(PetTests) {}

TEST(PetTests, DrawGeneratedPetOnScreenTest) {
}

TEST_GROUP_RUNNER(PetTests) {
  RUN_TEST_CASE(PetTests, DrawGeneratedPetOnScreenTest)
}
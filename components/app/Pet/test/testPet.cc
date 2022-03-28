#include "EspGLUtils.hpp"
#include "Pet.hpp"
#include "PetGenerator.hpp"
#include "unity.h"
#include "unity_fixture.h"

extern tamagotchi::EspGL::Screen<uint16_t> screen;
extern tamagotchi::EspGL::Vect2 center;

TEST_GROUP(PetTests);

TEST_SETUP(PetTests) {
  tamagotchi::EspGL::delay(1000);
  screen.fill(
      tamagotchi::EspGL::Colour<uint16_t>(tamagotchi::EspGL::colours::BLACK));
  tamagotchi::EspGL::delay(1000);
}

TEST_TEAR_DOWN(PetTests) {}

TEST(PetTests, DrawGeneratedPetOnScreenTest) {
  tamagotchi::App::Pet::PetGenerator<uint16_t> petGen("petComponents.pbm");
  auto pet = petGen.generate();
  tamagotchi::EspGL::Vect2 start(0, 0);
  pet.draw(screen, start);
}

TEST_GROUP_RUNNER(PetTests) {
  RUN_TEST_CASE(PetTests, DrawGeneratedPetOnScreenTest)
}
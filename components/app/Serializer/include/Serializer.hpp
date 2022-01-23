#pragma once
#include <stdint.h>

#include <sstream>

#include "SPIFFSDriver.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {
template <typename ColourRepresentation>
class Pet;
}
namespace Serializer {

class Serializer {
 public:
  virtual void serialize(Pet::Pet<uint16_t>* pet);

 private:
  SPIFFS::SPIFFSDriver spiffsDriver_;
};

}  // namespace Serializer
}  // namespace App
}  // namespace tamagotchi
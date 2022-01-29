#pragma once
#include <stdint.h>

#include <fstream>
#include <type_traits>
#include <vector>

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
  Serializer() = default;
  virtual void serialize(Pet::Pet<uint16_t>* pet,
                         std::string filename = "pet.ser");
  void serialize(std::fstream& fileHandle, std::string string);
  void deserialize(std::fstream& fileHandle, std::string& deserializedString);

  template <
      typename ArithmeticType,
      typename = typename std::enable_if<
          std::is_arithmetic<ArithmeticType>::value, ArithmeticType>::type>
  void serialize(std::fstream& fileHandle, ArithmeticType data) {
    size_t size = sizeof(data);
    fileHandle.write(reinterpret_cast<char*>(&size), sizeof(size));
    fileHandle.write(reinterpret_cast<char*>(&data), size);
  }

  template <
      typename ArithmeticType,
      typename = typename std::enable_if<
          std::is_arithmetic<ArithmeticType>::value, ArithmeticType>::type>
  void deserialize(std::fstream& fileHandle, ArithmeticType& data) {
    size_t size;
    fileHandle.read(reinterpret_cast<char*>(&size), sizeof(size));
    fileHandle.read(reinterpret_cast<char*>(&data), size);
  }

  template <typename T>
  void serialize(std::fstream& fileHandle, std::vector<T> data) {
    serialize(fileHandle, data.size());
    for (auto& elem : data) {
      serialize(fileHandle, elem);
    }
  }

 private:
  SPIFFS::SPIFFSDriver spiffsDriver_;
};

}  // namespace Serializer
}  // namespace App
}  // namespace tamagotchi
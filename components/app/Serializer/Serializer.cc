#include "Serializer.hpp"

#include "Pet.hpp"
namespace tamagotchi {
namespace App {
namespace Serializer {

template <>
void Serializer::serialize(std::fstream& fileHandle, std::vector<bool> data) {
  std::vector<uint8_t> boolData((data.size() + 7) / 8, 0);
  for (auto i = 0; i < data.size(); i += 8) {
    auto operationsNum = i + 8 < data.size() ? 8 : data.size() - i;
    uint8_t packedBools{0u};
    for (auto j = 0; j < operationsNum; i++) {
      packedBools += data[i + j] << j;
    }
    boolData[i / 8] = packedBools;
  }
  serialize(fileHandle, boolData);
}

void Serializer::serialize(std::fstream& fileHandle, std::string string) {
  auto stringLen = string.length();
  fileHandle.write(reinterpret_cast<char*>(&stringLen), sizeof(stringLen));
  fileHandle.write(string.data(), stringLen);
}

void Serializer::deserialize(std::fstream& fileHandle,
                             std::string& deserializedString) {
  size_t stringSize;
  fileHandle.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
  deserializedString.resize(stringSize);
  fileHandle.read(reinterpret_cast<char*>(deserializedString.data()),
                  stringSize);
}

void Serializer::serialize(Pet::Pet<uint16_t>* pet, std::string filename) {
  auto fileHandle = spiffsDriver_.createNewFile(filename);
  if (fileHandle.is_open()) {
    // NAME
    serialize(fileHandle, pet->name());
    // NEEDS
    auto needs = pet->needs();
    serialize(fileHandle, Pet::PET_NEEDS);
    for (int i = 0; i < Pet::PET_NEEDS; i++) {
      serialize(fileHandle, needs[i]);
    }
    // COLOUR
    serialize(fileHandle, pet->colour().value());
    // BODY
    auto body = pet->body();
    serialize(fileHandle, body.first);
    serialize(fileHandle, body.second.sizeX());
    serialize(fileHandle, body.second.sizeY());
    serialize(fileHandle, body.second.bitmap());
    // EYES
    auto eyes = pet->eyes();
    serialize(fileHandle, eyes.first);
    serialize(fileHandle, eyes.second.sizeX());
    serialize(fileHandle, eyes.second.sizeY());
    // FACE
    auto face = pet->face();
    serialize(fileHandle, face.first);
    serialize(fileHandle, face.second.sizeX());
    serialize(fileHandle, face.second.sizeY());
  }
}

}  // namespace Serializer
}  // namespace App
}  // namespace tamagotchi

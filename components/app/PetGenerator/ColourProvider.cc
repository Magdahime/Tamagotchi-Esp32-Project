#include "ColourProvider.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {

namespace ColourProvider {
  
template <>
std::vector<EspGL::Colour<uint16_t>> getPossibleColours<uint16_t>() {
  return std::vector<EspGL::Colour<uint16_t>>{
      EspGL::Colour<uint16_t>(0x8170), EspGL::Colour<uint16_t>(0x8710),
      EspGL::Colour<uint16_t>(0xfc9f), EspGL::Colour<uint16_t>(0x9ddf),
      EspGL::Colour<uint16_t>(0x05a7), EspGL::Colour<uint16_t>(0xf5b5),
      EspGL::Colour<uint16_t>(0xf5a2), EspGL::Colour<uint16_t>(0x25b7)};
}

}  // namespace ColourProvider
}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi
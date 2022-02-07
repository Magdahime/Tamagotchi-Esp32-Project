#pragma once

#include <stdint.h>

#include <vector>

#include "EspGLUtils.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {

namespace ColourProvider {

template <typename ColourRepresentation>
std::vector<EspGL::Colour<ColourRepresentation>> getPossibleColours();

template <>
std::vector<EspGL::Colour<uint16_t>> getPossibleColours<uint16_t>();

}  // namespace ColourProvider

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi
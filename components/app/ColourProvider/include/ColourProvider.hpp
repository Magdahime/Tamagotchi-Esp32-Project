#pragma once

#include <stdint.h>

#include <vector>

#include "EspGLUtils.hpp"

namespace tamagotchi {
namespace App {

namespace ColourProvider {

template <typename ColourRepresentation>
std::vector<EspGL::Colour<ColourRepresentation>> getPossiblePetColours();
template <>
std::vector<EspGL::Colour<uint16_t>> getPossiblePetColours<uint16_t>();

template <typename ColourRepresentation>
std::vector<EspGL::Colour<ColourRepresentation>> getPossibleGomokuColours();
template <>
std::vector<EspGL::Colour<uint16_t>> getPossibleGomokuColours<uint16_t>();

}  // namespace ColourProvider

}  // namespace App
}  // namespace tamagotchi
#pragma once

#include <stdint.h>

#include <string>
#include <utility>

#include "BitmapPet.hpp"
#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Pet.hpp"
#include "PetCommon.hpp"

namespace tamagotchi {
namespace App {
namespace Pet {

template <typename ColourRepresentation>
class DrawablePet : public Pet<ColourRepresentation>,
                    public BitmapPet<ColourRepresentation> {
 public:
  DrawablePet() = default;
  DrawablePet(std::pair<std::string, EspGL::Bitmap> body,
              std::pair<std::string, EspGL::Bitmap> eyes,
              std::pair<std::string, EspGL::Bitmap> face,
              EspGL::Colour<ColourRepresentation> colour, EspGL::Vect2 start,
              int scale = 5, std::string name = std::string())
      : Pet<ColourRepresentation>(name),
        BitmapPet<ColourRepresentation>(body, eyes, face, colour, start,
                                        scale) {}

  DrawablePet(Pet<ColourRepresentation> pet,
              BitmapPet<ColourRepresentation> bitmapPet)
      : Pet<ColourRepresentation>(std::move(pet)),
        BitmapPet<ColourRepresentation>(std::move(bitmapPet)) {}

  DrawablePet(BitmapPet<ColourRepresentation> bitmapPet)
      : Pet<ColourRepresentation>(),
        BitmapPet<ColourRepresentation>(std::move(bitmapPet)) {}

  ~DrawablePet() = default;
};

}  // namespace Pet
}  // namespace App
}  // namespace tamagotchi

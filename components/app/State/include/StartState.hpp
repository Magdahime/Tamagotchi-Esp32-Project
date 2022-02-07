#pragma once
#include "Event.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include "Graphics/Text/EspGLText.hpp"
#include "Pet.hpp"
#include "PetGenerator.hpp"
#include "Serializer.hpp"
#include "State.hpp"


namespace tamagotchi {

namespace App {

namespace State {

class StartState : public State {
 public:
  StartState() = default;
  virtual void handleEvent(Event::Event Event) override;
  virtual void run() override;

 private:
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
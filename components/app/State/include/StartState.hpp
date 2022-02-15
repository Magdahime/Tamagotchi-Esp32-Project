#pragma once
#include "Event.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Graphics/Text/EspGLText.hpp"
#include "Pet.hpp"
#include "PetGenerator.hpp"
#include "Serializer.hpp"
#include "State.hpp"
#include "esp_log.h"

namespace tamagotchi {

namespace App {

namespace State {

class StartState : public State {
 public:
  StartState() = default;
  virtual void handleEvent(Event::Event Event) override;
  virtual void run() override;

 private:
  static constexpr char TAG_[] = "StartState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
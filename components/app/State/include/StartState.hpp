#pragma once
#include <string>

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
  virtual ~StartState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  static constexpr char TAG_[] = "StartState";
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
#pragma once
#include <stdint.h>

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "Drawable.hpp"
#include "EspGLUtils.hpp"
#include "Event.hpp"
#include "State.hpp"
#include "StateUtils.hpp"

namespace tamagotchi {

namespace App {

namespace State {

class MainMenuState : public State {
 public:
  MainMenuState();
  virtual ~MainMenuState() = default;
  virtual void handleEvent(Event::Event event) override;
  virtual void stateInit() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  enum class Direction { FORWARDS, BACKWARDS };
  void handleGpioInput(int pressedButton);
  void deserializeIcons();
  void shiftIconPointer(Direction direction);
  static constexpr char TAG_[] = "MainMenuState";
  int currentSelection_;
  int lastSelection_;
  std::vector<EspGL::Hitbox> hitboxes_;
  std::vector<std::string> labels_;
  std::vector<StateType> stateTransitions_;
  std::map<std::string, std::unique_ptr<EspGL::Drawable<uint16_t>>> drawables_;
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
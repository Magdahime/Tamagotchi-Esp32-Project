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
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }

 private:
  void deserializeIcons();
  void shiftIconPointer();
  static constexpr char TAG_[] = "MainMenuState";
  std::vector<std::string> labels_;
  std::vector<std::string>::iterator iconPointer_;
  std::map<std::string, std::unique_ptr<EspGL::Drawable<uint16_t>>> drawables_;
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
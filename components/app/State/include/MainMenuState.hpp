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

namespace tamagotchi {

namespace App {

namespace State {

namespace consts {
constexpr auto ICON_COLOUR = EspGL::colours::WHITE;
constexpr std::pair<int, int> START_ICONS = {0, 0};
constexpr int ICONS_SPACING = 5;  // in pixels
constexpr int ICONS_SCALE = 3;
constexpr int STEP_X = 10;  // in pixels
constexpr int STEP_Y = 10;  // in pixels
constexpr int THRESHHOLD = 10;
}  // namespace consts

class MainMenuState : public State {
 public:
  MainMenuState();
  virtual ~MainMenuState() = default;
  virtual void handleEvent(Event::Event Event) override;
  virtual void init() override;
  virtual void mainLoop() override;
  virtual void deinit() override;

  virtual std::string toString() override { return TAG_; }
  void movePet();

 private:
  bool checkCollision(EspGL::Point newPoint);
  void deserializeIcons();
  static constexpr char TAG_[] = "MainMenuState";
  std::vector<std::string> labels_;
  std::map<std::string, std::unique_ptr<EspGL::Drawable<uint16_t>>> drawables_;
};

}  // namespace State

}  // namespace App

}  // namespace tamagotchi
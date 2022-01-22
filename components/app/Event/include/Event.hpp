#pragma once

#include <stdint.h>

#include "EspNowConf.hpp"
namespace tamagotchi {
namespace App {
namespace Event {

namespace consts {
constexpr int QUEUE_DATA_SIZE = 504;
}

namespace EventTypes {
constexpr uint8_t terminate = 0b00000000;
constexpr uint8_t espNow = 0b00000001;
constexpr uint8_t gpio = 0b00000010;
}  // namespace EventTypes

struct Event {
  uint8_t type_;
  uint8_t data_[consts::QUEUE_DATA_SIZE];
};

struct GpioEvent {
  bool state_;
  int gpioNum_;
};

struct EspNowEvent {
  uint8_t macAddress_[ESP_NOW_ETH_ALEN];
  int length_;
  uint8_t data_[EspNow::consts::ESPNOW_MAXMSGLENGTH];
};

}  // namespace Event
}  // namespace App
}  // namespace tamagotchi
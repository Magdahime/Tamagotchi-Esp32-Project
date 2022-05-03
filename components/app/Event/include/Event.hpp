#pragma once

#include <stdint.h>
#include <cstring>

namespace tamagotchi {
namespace App {
namespace Event {

namespace consts {
constexpr int QUEUE_DATA_SIZE = 504;
}

namespace EventTypes {
constexpr uint8_t terminate = 0b11111111;
constexpr uint8_t espNow = 0b00000001;
constexpr uint8_t gpio = 0b00000010;
}  // namespace EventTypes

struct Event {
  Event() : type_(0) { memset(data_, 0, consts::QUEUE_DATA_SIZE); }
  Event(uint8_t type) : type_(type) {}
  uint8_t type_;
  uint8_t data_[consts::QUEUE_DATA_SIZE];
  bool empty() { return type_ == 0; }
};


}  // namespace Event
}  // namespace App
}  // namespace tamagotchi
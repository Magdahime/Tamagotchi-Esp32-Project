#pragma once

#include "esp_now.h"
#include <cstring>
#include <stdint.h>

#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF ESP_IF_WIFI_AP

namespace tamagotchi {
namespace EspNow {
namespace consts {
constexpr char ESPNOW_PMK[] = "pmk1234567890123";
constexpr char ESPNOW_LMK[] = "lmk1234567890123";
constexpr int ESPNOW_CHANNEL = 1;
constexpr int ESPNOW_SEND_COUNT = 100;
constexpr int ESPNOW_SEND_DELAY = 1000;
constexpr int ESPNOW_SEND_LEN = 200;
constexpr int ESPNOW_QUEUE_SIZE = 6;
constexpr int ESPNOW_MAXDELAY = 512;
constexpr int ESPNOW_MAXMSGLENGTH = 256;
constexpr int ESPNOW_DATA_MAX = 2;
constexpr int ESPNOW_COMMUNICATION_METHODS = 2;
constexpr char TAG[] = "ESP32 EspnowDriver";
constexpr uint8_t EXAMPLE_BROADCAST_MAC[ESP_NOW_ETH_ALEN] = {0xFF, 0xFF, 0xFF,
                                                             0xFF, 0xFF, 0xFF};
constexpr uint16_t espnow_seq[ESPNOW_DATA_MAX] = {0, 0};
} // namespace consts

inline bool isBroadcastAddress(const void *address) {
  return memcmp(address, consts::EXAMPLE_BROADCAST_MAC, ESP_NOW_ETH_ALEN) == 0;
}


} // namespace EspNow

} // namespace tamagotchi

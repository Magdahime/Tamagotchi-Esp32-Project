#pragma once

#include "esp_now.h"
#include <cstring>
#include <stdint.h>

/* ESPNOW can work in both station and softap mode. It is configured in
 * menuconfig. */
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

namespace structs {
enum class espNowEventID {
  espNowSendCallback,
  espNowReceiveCallback,
};

typedef struct {
  uint8_t macAddress[ESP_NOW_ETH_ALEN];
  esp_now_send_status_t status;
} espNowEventSendCallback;

typedef struct {
  uint8_t macAddress[ESP_NOW_ETH_ALEN];
  uint8_t *data;
  int dataLength;
} espNowEventReceiveCallback;

typedef union {
  espNowEventSendCallback sendCallback;
  espNowEventReceiveCallback receiveCallback;
} espNowEventInfo;

typedef struct {
  espNowEventID id;
  espNowEventInfo info;
} espNowEvent;

enum class espNowCommunicationType {
  ESPNOW_DATA_BROADCAST,
  ESPNOW_DATA_UNICAST,
  ESPNOW_ERROR
};

typedef struct {
  espNowCommunicationType type; // Broadcast or unicast ESPNOW data.
  uint8_t state; // Indicate that if has received broadcast ESPNOW data or not.
  uint16_t sequenceNumber; // Sequence number of ESPNOW data.
  uint16_t crc;            // CRC16 value of ESPNOW data.
  uint32_t magic;     // Magic number which is used to determine which device to
                      // send unicast ESPNOW data.
  uint8_t payload[0]; // Real payload of ESPNOW data.
} __attribute__((packed)) espNowData;

typedef struct {
  bool unicast;   // Send unicast ESPNOW data.
  bool broadcast; // Send broadcast ESPNOW data.
  uint8_t state;  // Indicate that if has received broadcast ESPNOW data or not.
  uint32_t magic; // Magic number which is used to determine which device to
                  // send unicast ESPNOW data.
  uint16_t count; // Total count of unicast ESPNOW data to be sent.
  uint16_t delay; // Delay between sending two ESPNOW data, unit: ms.
  int len;        // Length of ESPNOW data to be sent, unit: byte.
  uint8_t *buffer; // Buffer pointing to ESPNOW data.
  uint8_t
      destinationMac[ESP_NOW_ETH_ALEN]; // MAC address of destination device.
} espNowParams;
} // namespace structs

} // namespace EspNow

} // namespace tamagotchi

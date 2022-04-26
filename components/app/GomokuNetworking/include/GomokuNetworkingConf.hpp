#pragma once
#include <variant>

#include "esp_now.h"

#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF ESP_IF_WIFI_AP

namespace tamagotchi {

namespace App {

namespace Gomoku {

namespace consts {
constexpr int MAX_GOMOKU_PLAYERS = 2;

constexpr char PMK[] = "pmk1234567890123";
constexpr char LMK[] = "lmk1234567890123";

constexpr int QUEUE_SIZE = 10;
constexpr int MAX_DELAY = 512;

constexpr int ESPNOW_CHANNEL = 1;
constexpr int ESPNOW_SEND_COUNT = 100;
constexpr int ESPNOW_SEND_DELAY = 1000;
constexpr int ESPNOW_SEND_LEN = 200;

constexpr int ESPNOW_MAXMSGLENGTH = 256;
constexpr int ESPNOW_DATA_MAX = 2;

constexpr uint8_t EXAMPLE_BROADCAST_MAC[ESP_NOW_ETH_ALEN] = {0xFF, 0xFF, 0xFF,
                                                             0xFF, 0xFF, 0xFF};
}  // namespace consts

namespace structs {

typedef struct {
  bool corrupted;
  bool unicast;
  uint8_t receiveState;
  uint16_t receiveSeq;
  int receiveMagic;
} receiveCallbackSummary;

enum class GomokuEventID {
  GomokuSendCallback,
  GomokuReceiveCallback,
};

typedef struct {
  esp_now_send_status_t status;
} GomokuEventSendCallback;

typedef struct {
  int dataLength;
  uint8_t *data;
} GomokuEventReceiveCallback;

typedef struct {
  GomokuEventID id;
  uint8_t macAddress[ESP_NOW_ETH_ALEN];
  std::variant<GomokuEventSendCallback, GomokuEventReceiveCallback> info;
} GomokuEvent;

enum class GomokuCommunicationType { BROADCAST, UNICAST, ERROR };

typedef struct {
  GomokuCommunicationType type;  // Broadcast or unicast ESPNOW data.
  uint8_t state;  // Indicate that if has received broadcast ESPNOW data or not.
  uint16_t sequenceNumber;  // Sequence number of ESPNOW data.
  uint16_t crc;             // CRC16 value of ESPNOW data.
  uint32_t magic;  // Magic number which is used to determine which device to
                   // send unicast ESPNOW data.
  uint8_t payload[0];  // Real payload of ESPNOW data.
} __attribute__((packed)) GomokuData;

typedef struct {
  bool unicast;    // Send unicast ESPNOW data.
  bool broadcast;  // Send broadcast ESPNOW data.
  uint8_t state;  // Indicate that if has received broadcast ESPNOW data or not.
  uint32_t magic;   // Magic number which is used to determine which device to
                    // send unicast ESPNOW data.
  uint16_t count;   // Total count of unicast ESPNOW data to be sent.
  uint16_t delay;   // Delay between sending two ESPNOW data, unit: ms.
  int len;          // Length of ESPNOW data to be sent, unit: byte.
  uint8_t *buffer;  // Buffer pointing to ESPNOW data.
  uint8_t
      destinationMac[ESP_NOW_ETH_ALEN];  // MAC address of destination device.
} GomokuParams;

}  // namespace structs

inline bool isBroadcastAddress(const void *address) {
  return memcmp(address, consts::EXAMPLE_BROADCAST_MAC, ESP_NOW_ETH_ALEN) == 0;
}

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi
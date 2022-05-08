#pragma once
#include <stdint.h>

#include <array>
#include <variant>

#include "esp_now.h"

#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF ESP_IF_WIFI_AP

namespace tamagotchi {

namespace App {

namespace Gomoku {
using mac_address_t = std::array<uint8_t, ESP_NOW_ETH_ALEN>;

namespace consts {
constexpr int MAX_GOMOKU_PLAYERS = 2;

constexpr char PMK[] = "pmk1234567890123";
constexpr char LMK[] = "lmk1234567890123";

constexpr int QUEUE_SIZE = 10;
constexpr int MAX_DELAY = 512;
constexpr int SENDING_INVITE_DELAY = 2000;

constexpr int ESPNOW_CHANNEL = 1;
constexpr int ESPNOW_SEND_COUNT = 100;
constexpr int ESPNOW_SEND_DELAY = 1000;
constexpr int ESPNOW_SEND_LEN = 200;

constexpr int ESPNOW_MAXMSGLENGTH = 256;

constexpr mac_address_t EXAMPLE_BROADCAST_MAC = {0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF};
}  // namespace consts

namespace structs {

struct ReceiveCallbackSummary {
  bool corrupted;
  bool unicast;
  uint8_t receiveState;
  uint16_t receiveSeq;
  int receiveMagic;
};

enum class GomokuEventID {
  GomokuSendCallback,
  GomokuReceiveCallback,
};

struct GomokuEventSendCallback {
  esp_now_send_status_t status;
};

struct GomokuEventReceiveCallback {
  int dataLength;
  uint8_t *data;
};

struct GomokuEvent {
  GomokuEventID id;
  uint8_t macAddress[ESP_NOW_ETH_ALEN];
  std::variant<GomokuEventSendCallback, GomokuEventReceiveCallback> info;
};

enum class GomokuCommunicationType { BROADCAST, UNICAST, ERROR };

struct GomokuData {
  GomokuCommunicationType type;  // Broadcast or unicast ESPNOW data.
  uint8_t
      state;  // Indicat4e that if has received broadcast ESPNOW data or not.
  uint16_t sequenceNumber;  // Sequence number of ESPNOW data.
  uint16_t crc;             // CRC16 value of ESPNOW data.
  uint32_t magic;  // Magic number which is used to determine which device to
                   // send unicast ESPNOW data.
  uint8_t payload[consts::ESPNOW_SEND_LEN];  // Real payload of ESPNOW data.
} __attribute__((packed));

struct GomokuParams {
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
};

}  // namespace structs

inline bool isBroadcastAddress(const void *address) {
  return memcmp(address, consts::EXAMPLE_BROADCAST_MAC.data(),
                ESP_NOW_ETH_ALEN) == 0;
}

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi
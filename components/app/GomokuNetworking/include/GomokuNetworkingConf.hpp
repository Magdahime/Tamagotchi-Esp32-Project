#pragma once
#include <stdint.h>

#include <array>
#include <cstring>
#include <optional>
#include <variant>

#include "esp_now.h"

#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF ESP_IF_WIFI_AP

namespace tamagotchi::App::Gomoku {

using mac_address_t = std::array<uint8_t, ESP_NOW_ETH_ALEN>;

struct BoardCoordinate {
  int16_t x_;
  int16_t y_;
};

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
constexpr int ESPNOW_PAYLOAD_MAX = 150;
constexpr int ESPNOW_SEND_META_LEN = 8;
constexpr int ESPNOW_SEND_LEN = ESPNOW_SEND_META_LEN + ESPNOW_PAYLOAD_MAX;

constexpr int ESPNOW_MAXMSGLENGTH = 256;

constexpr mac_address_t EXAMPLE_BROADCAST_MAC = {0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF};
}  // namespace consts

using gomoku_payload_array_t = std::array<uint8_t, consts::ESPNOW_PAYLOAD_MAX>;

namespace GomokuMessageStates {
constexpr uint8_t ERROR = 0b00000000;
constexpr uint8_t BROADCAST = 0b00000001;
constexpr uint8_t UNICAST = 0b00000010;
constexpr uint8_t ACK = 0b00000110;
constexpr uint8_t SENDING_CONFIG = 0b00001110;
constexpr uint8_t SENDING_MOVE = 0b00011110;
constexpr uint8_t SENDING_ORDER = 0b00111110;
constexpr uint8_t DEAD_PLAYER = 0b10000010;
constexpr uint8_t END_OF_GAME = 0b11111111;

}  // namespace GomokuMessageStates

namespace structs {

struct PetParams {
  uint8_t body;
  uint8_t eyes;
  uint8_t face;
};

enum class GomokuCommunicationType : uint8_t { BROADCAST, UNICAST, ERROR };

struct GomokuData {
  GomokuCommunicationType type;  // Broadcast or unicast ESPNOW data.
  uint8_t state;                 // Indicate which type of message is this.
  uint16_t crc;                  // CRC16 value of ESPNOW data.
  uint32_t magic;  // Magic number which is used to determine which device to
                   // send unicast ESPNOW data.
  gomoku_payload_array_t payload;
} __attribute__((packed));

static_assert(consts::ESPNOW_SEND_LEN == sizeof(GomokuData));

struct GomokuEventSendCallback {
  esp_now_send_status_t status;
};

struct GomokuEventReceiveCallback {
  GomokuData data;
};

struct GomokuEvent {
  mac_address_t macAddress;
  std::variant<GomokuEventSendCallback, GomokuEventReceiveCallback,
               std::monostate>
      info;
};

struct GomokuParams {
  bool unicast;    // Send unicast ESPNOW data.
  bool broadcast;  // Send broadcast ESPNOW data.
  uint8_t state;  // Indicate that if has received broadcast ESPNOW data or not.
  uint32_t magic;  // Magic number which is used to determine which device to
                   // send unicast ESPNOW data.
  uint16_t count;  // Total count of unicast ESPNOW data to be sent.
  uint16_t delay;  // Delay between sending two ESPNOW data, unit: ms.
  int len;         // Length of ESPNOW data to be sent, unit: byte.
  std::array<uint8_t, sizeof(GomokuData)>
      buffer;                    // Buffer pointing to ESPNOW data.
  mac_address_t destinationMac;  // MAC address of destination device.
};

struct GomokuDataWithRecipient {
  mac_address_t destinationMac;
  GomokuData data;
};

struct GomokuNextPlayerInOrder {
  mac_address_t nextPlayer;
};

struct GomokuMoveUpdateFromPlayer {
  mac_address_t player;
  BoardCoordinate move;
};

}  // namespace structs

inline bool isBroadcastAddress(const void *address) {
  return memcmp(address, consts::EXAMPLE_BROADCAST_MAC.data(),
                ESP_NOW_ETH_ALEN) == 0;
}

}  // namespace tamagotchi::App::Gomoku
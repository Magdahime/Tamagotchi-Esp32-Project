#pragma once
#include <stdint.h>

#include <array>
#include <cstring>
#include <optional>
#include <variant>
#include <vector>

#include "ColourProvider.hpp"
#include "MessageQueue.hpp"
#include "esp_now.h"

#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF ESP_IF_WIFI_AP

namespace tamagotchi::App::GomokuNetworking {

struct BoardCoordinate {
  int16_t x_;
  int16_t y_;
};
using mac_address_t = std::array<uint8_t, ESP_NOW_ETH_ALEN>;
using PlayerMove = std::pair<mac_address_t, BoardCoordinate>;

namespace consts {
constexpr int MAX_GOMOKU_PLAYERS = 2;
constexpr int MIN_GOMOKU_PLAYERS = 2;

constexpr char PMK[] = "pmk1234567890123";
constexpr char LMK[] = "lmk1234567890123";

constexpr int QUEUE_SIZE = 10;
constexpr int MAX_DELAY = 512;

constexpr int ESPNOW_CHANNEL = 1;
constexpr int ESPNOW_SEND_COUNT = 100;
constexpr int ESPNOW_SEND_DELAY = 100;

constexpr int ESPNOW_RETRANSMIT_THRESHOLD_MS = 1000;

constexpr int ESP_NOW_RETRANSMIT_MAX = 5;
constexpr int ESPNOW_PAYLOAD_MAX = 150;
constexpr int ESPNOW_SEND_META_LEN = 8;
constexpr int ESPNOW_SEND_LEN = ESPNOW_SEND_META_LEN + ESPNOW_PAYLOAD_MAX;

constexpr int MICRO2MILI = 1000;

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
constexpr uint8_t SENDING_COLOUR_CONFIG = 0b00001110;
constexpr uint8_t SENDING_MOVE_TO_PLAYERS = 0b00010010;
constexpr uint8_t SENDING_MOVE_TO_HOST = 0b00011110;
constexpr uint8_t SENDING_ORDER = 0b00111110;
constexpr uint8_t DEAD_PLAYER = 0b10000010;
constexpr uint8_t START_OF_GAME = 0b10000001;
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
  bool unicast;   // Send unicast ESPNOW data.
  uint8_t state;  // Indicate that if has received broadcast ESPNOW data or not.
  uint32_t magic;  // Magic number which is used to determine which device to
                   // send unicast ESPNOW data.
  int len;         // Length of ESPNOW data to be sent, unit: byte.
  std::array<uint8_t, sizeof(GomokuData)>
      buffer;                    // Buffer pointing to ESPNOW data.
  mac_address_t destinationMac;  // MAC address of destination device.
};

struct HostParams {
  bool newMove;
  bool acksCollected;
  MessageQueue::MessageQueue<mac_address_t> disconnectedPlayers;
};

struct SenderParams {
  mac_address_t macAddress;
  bool ack;
  int64_t timestamp;
  int retransmitCounter;
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

struct Colour2Player {
  uint16_t colourValue;
  mac_address_t player;
};

struct ColourConfig {
  std::array<Colour2Player, consts::MAX_GOMOKU_PLAYERS> colour2Player;
};

}  // namespace structs

inline bool isBroadcastAddress(const void *address) {
  return memcmp(address, consts::EXAMPLE_BROADCAST_MAC.data(),
                ESP_NOW_ETH_ALEN) == 0;
}

}  // namespace tamagotchi::App::GomokuNetworking
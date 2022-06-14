#include "GomokuNetworking.hpp"

#include <stdint.h>
#include <stdio.h>

#include <utility>
#include <variant>

#include "Game.hpp"
#include "Globals.hpp"
#include "esp_mac.h"
#include "nvs_flash.h"

namespace tamagotchi::App::GomokuNetworking {
///////////////////////////////////////////////////////////////////////////
std::vector<std::pair<mac_address_t, structs::PetParams>>
    GomokuNetworking::playersParams_ = {};
TaskHandle_t GomokuNetworking::gomokuNetworkingTask_ = nullptr;
MessageQueue::MessageQueue<structs::GomokuEvent>
    GomokuNetworking::receiveQueue_(10);
MessageQueue::MessageQueue<structs::GomokuDataWithRecipient>
    GomokuNetworking::sendingQueue_(consts::MAX_GOMOKU_PLAYERS);
MessageQueue::MessageQueue<structs::GomokuEvent> GomokuNetworking::gameQueue_(
    10);

mac_address_t GomokuNetworking::hostAddress_ = {};
mac_address_t GomokuNetworking::gameHostAddress_ = {};
structs::GomokuParams GomokuNetworking::sendParams_ = {
    .unicast = false,
    .state = GomokuMessageStates::BROADCAST,
    .magic = esp_random(),
    .len = consts::ESPNOW_SEND_LEN,
    {0},
    {0}};

structs::HostParams GomokuNetworking::hostParams_ = {
    .newMove = false,
    .acksCollected = false,
    .disconnectedPlayers{consts::MAX_GOMOKU_PLAYERS}};
bool GomokuNetworking::ifDeinit_ = false;

///////////////////////////////////////////////////////////////////////////

void GomokuNetworking::init() {
  esp_base_mac_addr_get(hostAddress_.data());
  esp_base_mac_addr_get(gameHostAddress_.data());
  sendParams_.destinationMac = consts::EXAMPLE_BROADCAST_MAC;
  ESP_LOGI(TAG_, "Successfully initialized networking module!");
}

TaskHandle_t &GomokuNetworking::run() {
  xTaskCreate(task, "gomokuNetworkingTask_", 4096,
              (void *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 1,
              &gomokuNetworkingTask_);
  return gomokuNetworkingTask_;
}

void GomokuNetworking::deinit() {
  esp_now_deinit();
  vTaskDelete(gomokuNetworkingTask_);
}

void GomokuNetworking::task(void *pvParameters) {
  TaskHandle_t taskToNotify = static_cast<TaskHandle_t>(pvParameters);
  ESP_LOGI(TAG_, "Created GomokuNetworking task!");
  searchForFriends();
  // LET KNOW THAT PLAYERS ARE READY
  xTaskNotifyGive(taskToNotify);
  if (gameHostAddress_ == hostAddress_) {
    handleCommunicationHost();
  } else {
    handleCommunicationPlayer();
  }

  deinit();
}

void GomokuNetworking::searchForFriends() {
  // Current number of players is 1, because it's us.
  int players = 1;
  addPeerESP(consts::EXAMPLE_BROADCAST_MAC.data());

  while (sendParams_.state == GomokuMessageStates::BROADCAST) {
    if (players == consts::MAX_GOMOKU_PLAYERS) break;
    sendGameInvite();
    auto msg = receiveQueue_.getQueue(consts::MAX_DELAY);
    if (std::holds_alternative<structs::GomokuEventReceiveCallback>(msg.info)) {
      ESP_LOGI(TAG_, "VARIANT: %d", msg.info.index());

      auto gomokuData = unpackData(msg);

      ESP_LOGI(TAG_, "RECEIVE  BROADCAST DATA FROM: " MACSTR ", LEN: %d",
               MAC2STR(msg.macAddress.data()), sizeof(gomokuData));

      if (gomokuData.state == GomokuMessageStates::ERROR) {
        ESP_LOGE(TAG_, "DATA FROM: " MACSTR " is corrupted.",
                 MAC2STR(msg.macAddress.data()));
        continue;
      }
      if (addPeer(msg.macAddress, gomokuData, players)) players++;
      ESP_LOGI(TAG_, "Current number of players: %d ", players);

      chooseHost(msg.macAddress, gomokuData);

      if (sendParams_.state == GomokuMessageStates::BROADCAST &&
          players == consts::MAX_GOMOKU_PLAYERS) {
        ESP_LOGI(TAG_, "Stopping sending data to broadcast.");
        sendParams_.state = GomokuMessageStates::UNICAST;
      }
    }
  }

  ESP_LOGI(TAG_, "Found players!");
  ESP_LOGI(TAG_, "Clearing EventQueue");
  receiveQueue_.clearQueue();
  ESP_LOGI(TAG_, "Leaving searchForFriends() method");
}

void GomokuNetworking::handleCommunicationHost() {
  structs::GomokuDataWithRecipient sendMsg;
  structs::GomokuEvent receiveMsg;
  std::vector<structs::SenderParams> sendersParams;
  std::transform(playersParams_.begin(), playersParams_.end(),
                 sendersParams.begin(), [](auto pair) -> structs::SenderParams {
                   return {.macAddress = pair.first,
                           .ack = true,
                           .timestamp = 0,
                           .retransmitCounter = 0};
                 });

  while (!ifDeinit_) {
    // CHECKING IF WE NEED TO RETRANSMIT
    retransmit(sendersParams, sendMsg);
    // SENDING
    if (sendingQueue_.getQueue(sendMsg, consts::ESPNOW_SEND_DELAY) == pdPASS) {
      auto it = std::find_if(
          sendersParams.begin(), sendersParams.end(), [&](const auto &params) {
            return params.macAddress == sendMsg.destinationMac;
          });
      if (it != sendersParams.end()) {
        it->ack = false;
        it->timestamp = xTaskGetTickCount();
        sendMessage(sendMsg);
      }
    }

    //  RECEIVING
    if (receiveQueue_.getQueue(receiveMsg, consts::ESPNOW_SEND_DELAY) ==
        pdPASS) {
      ESP_LOGI(TAG_, "Received message");
      auto it = std::find_if(
          sendersParams.begin(), sendersParams.end(), [&](const auto &params) {
            return params.macAddress == sendMsg.destinationMac;
          });
      if (it != sendersParams.end()) {
        it->timestamp = 0;
        it->retransmitCounter = 0;
        handleMessage(receiveMsg, sendersParams);
      }
    }
  }
}

void GomokuNetworking::retransmit(
    std::vector<structs::SenderParams> &sendersParams,
    structs::GomokuDataWithRecipient &msg) {
  // NOTHING TO DO ALL ACKS
  if (std::all_of(sendersParams.begin(), sendersParams.end(),
                  [](auto &params) { return params.ack; })) {
    return;
  }
  for (auto &senderParams : sendersParams) {
    if (senderParams.ack != true) {
      if (senderParams.timestamp != 0 &&
          (xTaskGetTickCount() - senderParams.timestamp) / configTICK_RATE_HZ >
              consts::ESPNOW_SEND_DELAY) {
        senderParams.retransmitCounter++;
        msg.destinationMac = senderParams.macAddress;
        sendMessage(msg);
      }
      if (senderParams.retransmitCounter >= consts::ESP_NOW_RETRANSMIT_MAX) {
        hostParams_.disconnectedPlayers.putQueue(senderParams.macAddress);
      }
    }
  }
}

void GomokuNetworking::handleCommunicationPlayer() {
  structs::GomokuDataWithRecipient sendMsg;
  structs::GomokuEvent receiveMsg;
  while (!ifDeinit_) {
    // SENDING
    if (sendingQueue_.getQueue(sendMsg) == pdPASS) {
      sendMessage(sendMsg);
    }

    // RECEIVING
    if (receiveQueue_.getQueue(receiveMsg) == pdPASS) {
      ESP_LOGI(TAG_, "Received message");
      gameQueue_.putQueue(receiveMsg);
    }
  }
}

void GomokuNetworking::sendMessage(structs::GomokuDataWithRecipient message) {
  ESP_LOGI(TAG_, "Sending message");
  auto result =
      esp_now_send(message.destinationMac.data(),
                   reinterpret_cast<uint8_t *>(&message.data), sendParams_.len);
  if (result != ESP_OK) {
    ESP_LOGE(TAG_, "ESPNOW SEND ERROR: %d", result);
  }
}

void GomokuNetworking::chooseHost(mac_address_t &peer,
                                  structs::GomokuData &data) {
  // CHOOSING HOST OF THE GAME
  if (data.magic > sendParams_.magic) {
    ESP_LOGI(TAG_,
             "Magic number from : " MACSTR
             " is higher: %d. My magic number is %d. Setting new game host.",
             MAC2STR(peer.data()), data.magic, sendParams_.magic);
    memcpy(gameHostAddress_.data(), peer.data(), ESP_NOW_ETH_ALEN);
  }
  // SAME MAGIC NUMBER
  if (data.magic == sendParams_.magic) {
    auto crcHost = esp_crc16_le(
        UINT16_MAX, reinterpret_cast<uint8_t const *>(hostAddress_.data()),
        ESP_NOW_ETH_ALEN);
    auto recvCrc =
        esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const *>(peer.data()),
                     ESP_NOW_ETH_ALEN);
    if (recvCrc > crcHost) {
      ESP_LOGI(TAG_, "Setting new game host.");
      memcpy(gameHostAddress_.data(), peer.data(), ESP_NOW_ETH_ALEN);
    }
  }
}

void GomokuNetworking::addPeerESP(const uint8_t *macAddress) {
  esp_now_peer_info_t peer;
  peer.channel = consts::ESPNOW_CHANNEL;
  peer.ifidx = static_cast<wifi_interface_t>(ESPNOW_WIFI_IF);
  peer.encrypt = isBroadcastAddress(macAddress) ? false : true;
  memcpy((&peer)->lmk, consts::LMK, ESP_NOW_KEY_LEN);
  memcpy((&peer)->peer_addr, macAddress, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
  ESP_LOGI(TAG_, "ADD PEER SUCCESS: " MACSTR, MAC2STR(macAddress));
}

bool GomokuNetworking::addPeer(mac_address_t &peer, structs::GomokuData &data,
                               int players) {
  // ADDING NEW PLAYER TO KNOWN PEERS
  if (players < consts::MAX_GOMOKU_PLAYERS &&
      data.type == structs::GomokuCommunicationType::BROADCAST &&
      esp_now_is_peer_exist(peer.data()) == false) {
    auto playerParams =
        reinterpret_cast<structs::PetParams *>(data.payload.data());
    playersParams_.push_back(std::make_pair(
        peer, structs::PetParams{playerParams->body, playerParams->eyes,
                                 playerParams->face}));
    addPeerESP(peer.data());
    return true;
  }
  return false;
}

void GomokuNetworking::sendGameInvite() {
  ESP_LOGI(TAG_, "SENDING GAME INVITE TO BROADCAST: " MACSTR,
           MAC2STR(sendParams_.destinationMac));
  prepareData();
  auto result = esp_now_send(sendParams_.destinationMac.data(),
                             sendParams_.buffer.data(), sendParams_.len);
  if (result != ESP_OK) {
    ESP_LOGE(TAG_, "ESPNOW SEND ERROR: %d", result);
  }
  vTaskDelay(consts::SENDING_INVITE_DELAY / portTICK_PERIOD_MS);
}

void GomokuNetworking::sendDataCallback(const uint8_t *macAddress,
                                        esp_now_send_status_t status) {
  structs::GomokuEvent event;
  event.info = structs::GomokuEventSendCallback();
  auto &sendCallback = std::get<structs::GomokuEventSendCallback>(event.info);
  if (macAddress == NULL) {
    ESP_LOGE(TAG_, "SEND CALLBACK ERROR - MAC ADDRESS NULL");
    return;
  }

  memcpy(event.macAddress.data(), macAddress, ESP_NOW_ETH_ALEN);
  sendCallback.status = status;

  if (receiveQueue_.putQueue(event) != pdTRUE) {
    ESP_LOGE(TAG_, "SEND QUEUE FAIL");
  }
}

void GomokuNetworking::receiveDataCallback(const uint8_t *macAddress,
                                           const uint8_t *data,
                                           const int length) {
  structs::GomokuEvent event;
  event.info = structs::GomokuEventReceiveCallback();
  auto &receiveCallback =
      std::get<structs::GomokuEventReceiveCallback>(event.info);
  if (macAddress == hostAddress_.data()) {
    return;
  }
  if (macAddress == NULL || data == NULL || length <= 0) {
    ESP_LOGE(TAG_, "RECEIVE CALLBACK ARGUMENT ERROR");
    return;
  }

  memcpy(event.macAddress.data(), macAddress, ESP_NOW_ETH_ALEN);
  memcpy(reinterpret_cast<uint8_t *>(&receiveCallback.data), data, length);
  if (receiveQueue_.putQueue(event) != pdTRUE) {
    ESP_LOGE(TAG_, "RECEIVE QUEUE FAIL");
  }
}

void GomokuNetworking::prepareData() {
  structs::GomokuData *buf =
      reinterpret_cast<structs::GomokuData *>(sendParams_.buffer.data());
  assert(sendParams_.len >= sizeof(structs::GomokuData));
  auto petParams = Globals::game.getPetParams();
  buf->type = isBroadcastAddress(sendParams_.destinationMac.data())
                  ? structs::GomokuCommunicationType::BROADCAST
                  : structs::GomokuCommunicationType::UNICAST;
  buf->state = sendParams_.state;
  buf->crc = 0;
  buf->magic = sendParams_.magic;
  memcpy(buf->payload.data(), reinterpret_cast<uint8_t *>(&petParams),
         sizeof(structs::PetParams));
  buf->crc =
      esp_crc16_le(UINT16_MAX, sendParams_.buffer.data(), sendParams_.len);
  ESP_LOGI(TAG_, "Prepare data CRC %d", buf->crc);
}

structs::GomokuData GomokuNetworking::unpackData(structs::GomokuEvent event) {
  auto gomokuData =
      std::get<structs::GomokuEventReceiveCallback>(event.info).data;
  int16_t receivedCrc = gomokuData.crc;
  gomokuData.crc = 0;
  int16_t calculatedCrc =
      esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const *>(&gomokuData),
                   sizeof(structs::GomokuData));
  if (receivedCrc != calculatedCrc) {
    ESP_LOGE(TAG_, "Message from: " MACSTR "is corrupted. CRCs are not equal.",
             MAC2STR(event.macAddress.data()));
    return {};
  }
  return gomokuData;
}

std::vector<mac_address_t> GomokuNetworking::playersMacs() {
  std::vector<mac_address_t> addr;
  std::transform(playersParams_.begin(), playersParams_.end(),
                 std::back_inserter(addr),
                 [](auto &pair) -> mac_address_t { return pair.first; });
  return addr;
}

void GomokuNetworking::handleMessage(
    structs::GomokuEvent &message,
    std::vector<structs::SenderParams> &sendersParams) {
  auto gomokuData = unpackData(message);
  auto it = std::find_if(sendersParams.begin(), sendersParams.end(),
                         [&](const auto &params) {
                           return params.macAddress == message.macAddress;
                         });
  switch (gomokuData.state) {
    case GomokuMessageStates::ACK:
      // If found change to true
      if (it != sendersParams.end()) {
        ESP_LOGI(TAG_, "ACK from: " MACSTR, MAC2STR(message.macAddress.data()));
        it->ack = true;
      }
      // Check if we have all ACKS
      if (std::all_of(sendersParams.begin(), sendersParams.end(),
                      [](auto &senderParams) { return senderParams.ack; })) {
        ESP_LOGI(TAG_, "All ACKs collected.");
        hostParams_.acksCollected = true;
      }
      break;

    case GomokuMessageStates::SENDING_MOVE_TO_HOST:
      hostParams_.newMove = true;
      gameQueue_.putQueue(message);
      break;

    // IS CORRUPTED
    default:
      break;
  }
}

void GomokuNetworking::startCollectingACKs() {
  hostParams_.acksCollected = false;
}

}  // namespace tamagotchi::App::GomokuNetworking

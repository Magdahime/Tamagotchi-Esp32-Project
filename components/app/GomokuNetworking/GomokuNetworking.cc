#include "GomokuNetworking.hpp"

#include <stdint.h>
#include <stdio.h>

#include <variant>

#include "esp_mac.h"
#include "nvs_flash.h"

namespace tamagotchi::App::Gomoku {
///////////////////////////////////////////////////////////////////////////
std::vector<mac_address_t> GomokuNetworking::playersMacs_ = {};
TaskHandle_t GomokuNetworking::gomokuNetworkingTask_ = nullptr;
MessageQueue::MessageQueue<structs::GomokuEvent>
    GomokuNetworking::receiveQueue_(10);
MessageQueue::MessageQueue<structs::GomokuDataWithRecipient>
    GomokuNetworking::sendingQueue_(consts::MAX_GOMOKU_PLAYERS);

mac_address_t GomokuNetworking::hostAddress_ = {};
mac_address_t GomokuNetworking::gameHostAddress_ = {};
structs::GomokuParams GomokuNetworking::sendParams_ = {
    .unicast = false,
    .broadcast = true,
    .state = GomokuMessageStates::BROADCAST,
    .magic = esp_random(),
    .count = 0,
    .delay = consts::ESPNOW_SEND_DELAY,
    .len = consts::ESPNOW_SEND_LEN,
    {0},
    {0}};

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
  handleCommunication();
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
      auto &receiveCallback =
          std::get<structs::GomokuEventReceiveCallback>(msg.info);

      auto summary = parseData(receiveCallback.data);
      ESP_LOGI(TAG_, "RECEIVE %dth BROADCAST DATA FROM: " MACSTR ", LEN: %d",
               summary.receiveSeq, MAC2STR(msg.macAddress.data()),
               sizeof(structs::GomokuData));
      if (summary.corrupted == true) {
        ESP_LOGE(TAG_, "DATA FROM: " MACSTR " is corrupted.",
                 MAC2STR(msg.macAddress.data()));
        continue;
      }
      if (addPeer(msg.macAddress, summary, players)) players++;
      ESP_LOGI(TAG_, "Current number of players: %d ", players);

      chooseHost(msg.macAddress, summary);

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

void GomokuNetworking::handleCommunication() {
  structs::GomokuDataWithRecipient msg;
  while (!ifDeinit_ && sendingQueue_.getQueue(msg) == pdPASS) {
    auto result =
        esp_now_send(msg.destinationMac.data(),
                     reinterpret_cast<uint8_t *>(&msg.data), sendParams_.len);
    if (result != ESP_OK) {
      ESP_LOGE(TAG_, "ESPNOW SEND ERROR: %d", result);
    }
  }
}

void GomokuNetworking::chooseHost(mac_address_t &peer,
                                  structs::ReceiveCallbackSummary &summary) {
  // CHOOSING HOST OF THE GAME
  if (summary.receiveMagic > sendParams_.magic) {
    ESP_LOGI(TAG_,
             "Magic number from : " MACSTR
             " is higher: %d. My magic number is %d. Setting new game host.",
             MAC2STR(peer.data()), summary.receiveMagic, sendParams_.magic);
    memcpy(gameHostAddress_.data(), peer.data(), ESP_NOW_ETH_ALEN);
  }
  // SAME MAGIC NUMBER
  if (summary.receiveMagic == sendParams_.magic) {
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

bool GomokuNetworking::addPeer(mac_address_t &peer,
                               structs::ReceiveCallbackSummary &summary,
                               int players) {
  // ADDING NEW PLAYER TO KNOWN PEERS
  if (players < consts::MAX_GOMOKU_PLAYERS && summary.unicast == false &&
      esp_now_is_peer_exist(peer.data()) == false) {
    playersMacs_.push_back(peer);
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

void GomokuNetworking::sendData(const uint8_t *macAddress,
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

void GomokuNetworking::receiveData(const uint8_t *macAddress,
                                   const uint8_t *data, const int length) {
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

structs::ReceiveCallbackSummary GomokuNetworking::parseData(
    structs::GomokuData data) {
  structs::ReceiveCallbackSummary summary;
  uint16_t receivedCrc, calculatedCrc;
  summary.corrupted = true;
  summary.receiveState = data.state;
  summary.receiveSeq = data.sequenceNumber;
  summary.receiveMagic = data.magic;

  receivedCrc = data.crc;
  data.crc = 0;
  calculatedCrc =
      esp_crc16_le(UINT16_MAX, reinterpret_cast<uint8_t const *>(&data),
                   sizeof(structs::GomokuData));

  ESP_LOGI(TAG_, "RECEIVED %d, CALCULATED %d", receivedCrc, calculatedCrc);

  if (calculatedCrc == receivedCrc) {
    summary.corrupted = false;
    summary.unicast =
        data.type == structs::GomokuCommunicationType::UNICAST ? true : false;
  }

  return summary;
}

void GomokuNetworking::prepareData() {
  structs::GomokuData *buf =
      reinterpret_cast<structs::GomokuData *>(sendParams_.buffer.data());
  assert(sendParams_.len >= sizeof(structs::GomokuData));

  buf->type = isBroadcastAddress(sendParams_.destinationMac.data())
                  ? structs::GomokuCommunicationType::BROADCAST
                  : structs::GomokuCommunicationType::UNICAST;
  buf->state = sendParams_.state;
  buf->sequenceNumber = 0;
  buf->crc = 0;
  buf->magic = sendParams_.magic;
  buf->crc =
      esp_crc16_le(UINT16_MAX, sendParams_.buffer.data(), sendParams_.len);
  ESP_LOGI(TAG_, "Prepare data CRC %d", buf->crc);
}

gomoku_payload_array_t GomokuNetworking::unpackData(
    structs::GomokuEvent event) {
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
  return gomokuData.payload;
}

}  // namespace tamagotchi::App::Gomoku

#include "GomokuNetworking.hpp"

#include <stdint.h>
#include <stdio.h>

#include "esp_mac.h"
#include "nvs_flash.h"

namespace tamagotchi {

namespace App {

namespace Gomoku {

std::vector<mac_address_t> GomokuNetworking::playersMacs_ = {};
TaskHandle_t GomokuNetworking::gomokuNetworkingTask = nullptr;
MessageQueue::MessageQueue<structs::GomokuEvent> GomokuNetworking::gomokuQueue_(
    10);

mac_address_t GomokuNetworking::hostAddress_ = {};
mac_address_t GomokuNetworking::gameHostAddress_ = {};
structs::GomokuParams GomokuNetworking::sendParams_ = {
    false,
    true,
    0,
    esp_random(),
    0,
    consts::ESPNOW_SEND_DELAY,
    consts::ESPNOW_SEND_LEN,
    new uint8_t[consts::ESPNOW_SEND_LEN],
    {0}};

void GomokuNetworking::init() {
  esp_base_mac_addr_get(hostAddress_.data());
  esp_base_mac_addr_get(gameHostAddress_.data());
  memcpy(sendParams_.destinationMac, consts::EXAMPLE_BROADCAST_MAC.data(),
         ESP_NOW_ETH_ALEN);
  ESP_LOGI(TAG_, "Successfully initialized networking module!");
}

TaskHandle_t &GomokuNetworking::run() {
  xTaskCreate(task, "GomokuNetworkingTask", 4096,
              (void *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 1,
              &gomokuNetworkingTask);
  return gomokuNetworkingTask;
}

void GomokuNetworking::deinit() {
  delete sendParams_.buffer;
  esp_now_deinit();
  vTaskDelete(gomokuNetworkingTask);
}

void GomokuNetworking::task(void *pvParameters) {
  TaskHandle_t taskToNotify = static_cast<TaskHandle_t>(pvParameters);
  ESP_LOGI(TAG_, "Created GomokuNetworking task!");
  searchForFriends();
  ESP_LOGI(TAG_, "Found players!");
  // LET KNOW THAT PLAYERS ARE READY
  xTaskNotifyGive(taskToNotify);

  deinit();
}

void GomokuNetworking::searchForFriends() {
  // Current number of players is 1, because it's us.
  int players = 1;
  addPeerESP(consts::EXAMPLE_BROADCAST_MAC.data());
  structs::GomokuEventReceiveCallback receiveCallback;
  while (sendParams_.state != 1) {
    if (players == consts::MAX_GOMOKU_PLAYERS) break;
    sendGameInvite();
    auto msg = gomokuQueue_.getQueue(consts::MAX_DELAY);
    if (msg.id == structs::GomokuEventID::GomokuReceiveCallback) {
      mac_address_t senderMac;
      std::copy(std::begin(msg.macAddress), std::end(msg.macAddress),
                senderMac.begin());
      receiveCallback = std::get<structs::GomokuEventReceiveCallback>(msg.info);
      auto summary = parseData(
          reinterpret_cast<structs::GomokuData *>(receiveCallback.data),
          receiveCallback.dataLength);
      ESP_LOGI(TAG_, "RECEIVE %dth BROADCAST DATA FROM: " MACSTR ", LEN: %d",
               summary.receiveSeq, MAC2STR(senderMac.data()),
               receiveCallback.dataLength);
      if (summary.corrupted == true) {
        ESP_LOGE(TAG_, "DATA FROM: " MACSTR " is corrupted.",
                 MAC2STR(senderMac.data()));
        continue;
      }
      if (addPeer(senderMac, summary, players)) players++;
      ESP_LOGI(TAG_, "Current number of players: %d ", players);

      chooseHost(senderMac, summary);

      if (sendParams_.state == 0 && players == consts::MAX_GOMOKU_PLAYERS) {
        ESP_LOGI(TAG_, "Stopping sending data to broadcast.");
        sendParams_.state = 1;
      }
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
        UINT16_MAX, (uint8_t const *)hostAddress_.data(), ESP_NOW_ETH_ALEN);
    auto recvCrc = esp_crc16_le(UINT16_MAX, (uint8_t const *)peer.data(),
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
  auto result = esp_now_send(sendParams_.destinationMac, sendParams_.buffer,
                             sendParams_.len);
  if (result != ESP_OK) {
    ESP_LOGE(TAG_, "ESPNOW SEND ERROR: %d", result);
  }
  vTaskDelay(consts::SENDING_INVITE_DELAY / portTICK_PERIOD_MS);
}

void GomokuNetworking::sendData(const uint8_t *macAddress,
                                esp_now_send_status_t status) {
  structs::GomokuEvent event;
  event.info = structs::GomokuEventSendCallback{};
  structs::GomokuEventSendCallback *sendCallback =
      &std::get<structs::GomokuEventSendCallback>(event.info);
  if (macAddress == NULL) {
    ESP_LOGE(TAG_, "SEND CALLBACK ERROR - MAC ADDRESS NULL");
    return;
  }

  event.id = structs::GomokuEventID::GomokuSendCallback;
  memcpy(event.macAddress, macAddress, ESP_NOW_ETH_ALEN);
  sendCallback->status = status;

  if (gomokuQueue_.putQueue(event) != pdTRUE) {
    ESP_LOGE(TAG_, "SEND QUEUE FAIL");
  }
}

void GomokuNetworking::receiveData(const uint8_t *macAddress,
                                   const uint8_t *data, const int length) {
  structs::GomokuEvent event;
  event.info = structs::GomokuEventReceiveCallback{};
  structs::GomokuEventReceiveCallback *receiveCallback =
      &std::get<structs::GomokuEventReceiveCallback>(event.info);
  if (macAddress == hostAddress_.data()) {
    return;
  }
  if (macAddress == NULL || data == NULL || length <= 0) {
    ESP_LOGE(TAG_, "RECEIVE CALLBACK ARGUMENT ERROR");
    return;
  }

  event.id = structs::GomokuEventID::GomokuReceiveCallback;
  memcpy(event.macAddress, macAddress, ESP_NOW_ETH_ALEN);
  receiveCallback->data = new uint8_t[length];
  if (receiveCallback->data == NULL) {
    ESP_LOGE(TAG_, "MALLOC RECEIVE DATA FAILURE");
    return;
  }

  memcpy(receiveCallback->data, data, length);
  receiveCallback->dataLength = length;
  if (gomokuQueue_.putQueue(event) != pdTRUE) {
    ESP_LOGE(TAG_, "RECEIVE QUEUE FAIL");
    delete[] receiveCallback->data;
  }
}

structs::ReceiveCallbackSummary GomokuNetworking::parseData(
    structs::GomokuData *data, int dataLength) {
  structs::ReceiveCallbackSummary summary;
  uint16_t receivedCrc, calculatedCrc;
  summary.corrupted = true;

  if (dataLength < sizeof(structs::GomokuData)) {
    ESP_LOGE(TAG_, "RECEIVED ESP NOW DATA IS TOO SHORT, len:%d", dataLength);
    return summary;
  }

  summary.receiveState = data->state;
  summary.receiveSeq = data->sequenceNumber;
  summary.receiveMagic = data->magic;

  receivedCrc = data->crc;
  data->crc = 0;
  calculatedCrc = esp_crc16_le(UINT16_MAX, (uint8_t const *)data, dataLength);

  if (calculatedCrc == receivedCrc) {
    summary.corrupted = false;
    summary.unicast =
        data->type == structs::GomokuCommunicationType::UNICAST ? true : false;
  }

  return summary;
}

void GomokuNetworking::prepareData() {
  structs::GomokuData *buf =
      reinterpret_cast<structs::GomokuData *>(sendParams_.buffer);
  assert(sendParams_.len >= sizeof(structs::GomokuData));

  buf->type = isBroadcastAddress(sendParams_.destinationMac)
                  ? structs::GomokuCommunicationType::BROADCAST
                  : structs::GomokuCommunicationType::UNICAST;
  buf->state = sendParams_.state;
  buf->sequenceNumber = 0;
  buf->crc = 0;
  buf->magic = sendParams_.magic;
  buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, sendParams_.len);
}

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi

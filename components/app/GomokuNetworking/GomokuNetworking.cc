#include "GomokuNetworking.hpp"

#include <stdint.h>
#include <stdio.h>

#include "esp_mac.h"
#include "nvs_flash.h"

namespace tamagotchi {

namespace App {

namespace Gomoku {

TaskHandle_t GomokuNetworking::gomokuNetworkingTask = nullptr;
MessageQueue::MessageQueue<structs::GomokuEvent> GomokuNetworking::gomokuQueue_(
    10);
SemaphoreHandle_t GomokuNetworking::mutex_ = xSemaphoreCreateBinary();
uint8_t GomokuNetworking::hostAddress_[ESP_NOW_ETH_ALEN] = {};
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
  esp_base_mac_addr_get(hostAddress_);
  memcpy(sendParams_.destinationMac, consts::EXAMPLE_BROADCAST_MAC,
         ESP_NOW_ETH_ALEN);
  xSemaphoreGive(mutex_);
  ESP_LOGI(TAG_, "Successfully initialized networking module!");
}

void GomokuNetworking::run() {
  xTaskCreate(task, "GomokuNetworkingTask", 4096, NULL,
              configMAX_PRIORITIES - 1, &gomokuNetworkingTask);
}

void GomokuNetworking::deinit() {
  delete sendParams_.buffer;
  esp_now_deinit();
  vTaskDelete(gomokuNetworkingTask);
}

void GomokuNetworking::task(void *pvParameters) {
  ESP_LOGI(TAG_, "Created GomokuNetworking task!");
  searchForFriends();
  ESP_LOGI(TAG_, "Found players!");
  // LET KNOW THAT PLAYERS ARE READY
  xSemaphoreGive(mutex_);
}

void GomokuNetworking::searchForFriends() {
  // Current number of players is 1, because it's us.
  int players = 1;
  addPeer(consts::EXAMPLE_BROADCAST_MAC);
  structs::GomokuEventReceiveCallback receiveCallback;
  while (true) {
    if (players == consts::MAX_GOMOKU_PLAYERS) break;
    sendGameInvite(sendParams_);
    auto msg = gomokuQueue_.getQueue(consts::MAX_DELAY);
    if (msg.id == structs::GomokuEventID::GomokuReceiveCallback) {
      receiveCallback = std::get<structs::GomokuEventReceiveCallback>(msg.info);
      auto summary = parseData(
          reinterpret_cast<structs::GomokuData *>(receiveCallback.data),
          receiveCallback.dataLength);
      ESP_LOGI(TAG_, "RECEIVE %dthBROADCAST DATA FROM: " MACSTR ", LEN: %d",
               summary.receiveSeq, MAC2STR(msg.macAddress),
               receiveCallback.dataLength);

      if (summary.corrupted == true) continue;
      if (players < consts::MAX_GOMOKU_PLAYERS && summary.unicast == false &&
          esp_now_is_peer_exist(msg.macAddress) == false) {
        addPeer(msg.macAddress);
        players++;
      }

      if (summary.receiveMagic > sendParams_.magic) {
        memcpy(hostAddress_, msg.macAddress, ESP_NOW_ETH_ALEN);
      }

      if (sendParams_.state == 0 && players == consts::MAX_GOMOKU_PLAYERS) {
        sendParams_.state = 1;
        break;
      }
    }
  }
}

void GomokuNetworking::addPeer(const uint8_t *macAddress) {
  esp_now_peer_info_t peer;
  peer.channel = consts::ESPNOW_CHANNEL;
  peer.ifidx = static_cast<wifi_interface_t>(ESPNOW_WIFI_IF);
  peer.encrypt = isBroadcastAddress(macAddress) ? false : true;
  memcpy((&peer)->lmk, consts::LMK, ESP_NOW_KEY_LEN);
  memcpy((&peer)->peer_addr, macAddress, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
  ESP_LOGI(TAG_, "ADD PEER SUCCESS: " MACSTR, MAC2STR(macAddress));
}

void GomokuNetworking::sendGameInvite(structs::GomokuParams &sendParams) {
  ESP_LOGI(TAG_, "SENDING GAME INVITE TO BROADCAST");
  auto result = esp_now_send(sendParams.destinationMac, sendParams.buffer,
                             sendParams.len);
  if (result != ESP_OK) {
    ESP_LOGE(TAG_, "ESPNOW SEND ERROR: %d", result);
  }
}

void GomokuNetworking::sendData(const uint8_t *macAddress,
                                esp_now_send_status_t status) {
  structs::GomokuEvent event;
  structs::GomokuEventSendCallback *sendCallback =
      &std::get<structs::GomokuEventSendCallback>(event.info);
  if (macAddress == NULL) {
    ESP_LOGE(TAG_, "SEND CALLBACK ERROR - MAC ADDRESS NULL");
    return;
  }

  event.id = structs::GomokuEventID::GomokuSendCallback;
  memcpy(event.macAddress, macAddress, ESP_NOW_ETH_ALEN);
  sendCallback->status = status;
  if (xQueueSend(gomokuQueue_.messageQueue(), &event, consts::MAX_DELAY) !=
      pdTRUE) {
    ESP_LOGE(TAG_, "SEND QUEUE FAIL");
  }
}

void GomokuNetworking::receiveData(const uint8_t *macAddress,
                                   const uint8_t *data, const int length) {
  structs::GomokuEvent event;
  structs::GomokuEventReceiveCallback *receiveCallback =
      &std::get<structs::GomokuEventReceiveCallback>(event.info);

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
  if (xQueueSend(gomokuQueue_.messageQueue(), &event, consts::MAX_DELAY) !=
      pdTRUE) {
    ESP_LOGE(TAG_, "RECEIVE QUEUE FAIL");
    delete[] receiveCallback->data;
  }
}

structs::receiveCallbackSummary GomokuNetworking::parseData(
    structs::GomokuData *data, int dataLength) {
  structs::receiveCallbackSummary summary;
  summary.corrupted = true;

  if (dataLength < sizeof(structs::GomokuData)) {
    ESP_LOGE(TAG_, "RECEIVED ESP NOW DATA IS TOO SHORT, len:%d", dataLength);
    return summary;
  }

  summary.receiveState = data->state;
  summary.receiveSeq = data->sequenceNumber;
  summary.receiveMagic = data->magic;

  uint16_t crc_cal =
      esp_crc16_le(UINT16_MAX, (uint8_t const *)data, dataLength);

  if (crc_cal == data->crc) {
    summary.corrupted = false;
    summary.unicast =
        data->type == structs::GomokuCommunicationType::UNICAST ? true : false;
  }

  return summary;
}

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi

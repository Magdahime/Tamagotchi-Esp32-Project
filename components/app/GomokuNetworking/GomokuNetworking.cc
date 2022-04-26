#include "GomokuNetworking.hpp"

#include <stdint.h>

#include "esp_mac.h"

namespace tamagotchi {

namespace App {

namespace Gomoku {

MessageQueue::MessageQueue<structs::GomokuEvent> GomokuNetworking::gomokuQueue_(
    10);

GomokuNetworking::GomokuNetworking()
    : players_(1),
      sendParams_{false,
                  true,
                  0,
                  esp_random(),
                  0,
                  consts::ESPNOW_SEND_DELAY,
                  consts::ESPNOW_SEND_LEN,
                  nullptr,
                  {0}} {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
  ESP_ERROR_CHECK(esp_wifi_start());

  /* Initialize ESPNOW and register sending and receiving callback function. */
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_send_cb(sendData));
  ESP_ERROR_CHECK(esp_now_register_recv_cb(receiveData));

  esp_base_mac_addr_get(hostAddress_);
  memcpy(sendParams_.destinationMac, consts::EXAMPLE_BROADCAST_MAC,
         ESP_NOW_ETH_ALEN);

  ESP_LOGI(TAG_, "Successfully initialized sending parameters!");
}

void GomokuNetworking::searchForFriends() {
  structs::GomokuEventReceiveCallback receiveCallback;
  while (true) {
    if (players_ == consts::MAX_GOMOKU_PLAYERS) break;
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
      if (players_ < consts::MAX_GOMOKU_PLAYERS && summary.unicast == false &&
          esp_now_is_peer_exist(msg.macAddress) == false) {
        addPeer(msg.macAddress);
        players_++;
      }

      if (summary.receiveMagic > sendParams_.magic) {
        memcpy(hostAddress_, msg.macAddress, ESP_NOW_ETH_ALEN);
      }

      if (sendParams_.state == 0 && players_ == consts::MAX_GOMOKU_PLAYERS) {
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
  peer.encrypt = true;
  memcpy((&peer)->lmk, consts::LMK, ESP_NOW_KEY_LEN);
  memcpy((&peer)->peer_addr, macAddress, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
  ESP_LOGE(TAG_, "ADD PEER SUCCESS");
}

void GomokuNetworking::sendGameInvite(structs::GomokuParams &sendParams) {
  ESP_LOGI(TAG_, "SENDING GAME INVITE TO BROADCAST");
  if (esp_now_send(sendParams.destinationMac, sendParams.buffer,
                   sendParams.len) != ESP_OK) {
    ESP_LOGE(TAG_, "ESPNOW SEND ERROR");
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

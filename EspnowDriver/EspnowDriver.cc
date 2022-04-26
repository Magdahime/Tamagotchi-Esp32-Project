#include "EspnowDriver.hpp"

namespace tamagotchi {
namespace EspNow {
xQueueHandle EspnowDriver::ESPNOWqueue_ = nullptr;
int64_t EspnowDriver::messageCounter_[consts::ESPNOW_COMMUNICATION_METHODS] = {
    0, 0};
int EspnowDriver::init() {
  wifiInit();
  ESPNOWqueue_ =
      xQueueCreate(consts::ESPNOW_QUEUE_SIZE, sizeof(structs::espNowEvent));
  if (ESPNOWqueue_ == NULL) {
    ESP_LOGE(consts::TAG, "CREATE MUTEX FAIL");
    return ESP_FAIL;
  }

  /* Initialize ESPNOW and register sending and receiving callback function. */
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_send_cb(sendData));
  ESP_ERROR_CHECK(esp_now_register_recv_cb(receiveData));

  /* Set primary master key. */
  ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)consts::ESPNOW_PMK));

  /* Adding broadcast to known peers*/
  addBroadcastPeer();

  /* Initialize sending parameters. */
  structs::espNowParams *sendParams =
      initializeSendingParameters(false, 0, consts::EXAMPLE_BROADCAST_MAC);
  if (sendParams == nullptr) {
    ESP_LOGE(consts::TAG, "CREATE SEND PARAMETERS FAILURE");
    return ESP_FAIL;
  }
  prepareData(sendParams);

  xTaskCreate(ESPNOWtask, "espnowTask", 2048, sendParams, 4, NULL);
  return ESP_OK;
}

void EspnowDriver::deinit(structs::espNowParams *params) {
  delete params->buffer;
  delete params;
  vSemaphoreDelete(ESPNOWqueue_);
  esp_now_deinit();
}

void EspnowDriver::wifiInit() {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
  ESP_ERROR_CHECK(esp_wifi_start());
}



structs::espNowCommunicationType
EspnowDriver::parseData(structs::espNowData *data, uint16_t dataLength,
                        uint8_t *state, uint16_t *sequence, int *magic) {
  uint16_t crc, crc_cal = 0;

  if (dataLength < sizeof(structs::espNowData)) {
    ESP_LOGE(consts::TAG, "RECEIVED ESP NOW DATA IS TOO SHORT, len:%d",
             dataLength);
    return structs::espNowCommunicationType::ESPNOW_ERROR;
  }

  *state = data->state;
  *sequence = data->sequenceNumber;
  *magic = data->magic;
  crc = data->crc;
  data->crc = 0;
  crc_cal = esp_crc16_le(UINT16_MAX, (uint8_t const *)data, dataLength);

  if (crc_cal == crc) {
    return data->type;
  }
  return structs::espNowCommunicationType::ESPNOW_ERROR;
}

void EspnowDriver::prepareData(structs::espNowParams *sendParams) {
  structs::espNowData *buf = (structs::espNowData *)sendParams->buffer;
  assert(sendParams->len >= sizeof(structs::espNowData));
  buf->type = isBroadcastAddress(sendParams->destinationMac)
                  ? structs::espNowCommunicationType::ESPNOW_DATA_BROADCAST
                  : structs::espNowCommunicationType::ESPNOW_DATA_UNICAST;
  buf->state = sendParams->state;
  buf->sequenceNumber = messageCounter_[static_cast<int>(buf->type)]++;
  buf->crc = 0;
  buf->magic = sendParams->magic;
  /* Fill all remaining bytes after the data with random values */
  esp_fill_random(buf->payload, sendParams->len - sizeof(structs::espNowData));
  buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, sendParams->len);
}

void EspnowDriver::ESPNOWtask(void *pvParameter) {
  structs::espNowEvent event;
  uint8_t recv_state = 0;
  uint16_t recv_seq = 0;
  int recv_magic = 0;
  bool is_broadcast = false;
  structs::espNowCommunicationType ret;

  vTaskDelay(5000 / portTICK_RATE_MS);
  ESP_LOGD(consts::TAG, "Start sending broadcast data");
  /* Start sending broadcast ESPNOW data. */
  structs::espNowParams *sendParams = (structs::espNowParams *)pvParameter;
  if (esp_now_send(sendParams->destinationMac, sendParams->buffer,
                   sendParams->len) != ESP_OK) {
    ESP_LOGE(consts::TAG, "ESPNOW SEND ERROR");
    deinit(sendParams);
    vTaskDelete(NULL);
  }

  while (xQueueReceive(ESPNOWqueue_, &event, portMAX_DELAY) == pdTRUE) {
    switch (event.id) {
    case structs::espNowEventID::espNowSendCallback: {
      structs::espNowEventSendCallback *sendCallback = &event.info.sendCallback;
      is_broadcast = isBroadcastAddress(sendCallback->macAddress);

      ESP_LOGD(consts::TAG, "Send data to " MACSTR ", status1: %d",
               MAC2STR(sendCallback->macAddress), sendCallback->status);

      if (is_broadcast && (sendParams->broadcast == false)) {
        break;
      }

      if (!is_broadcast) {
        sendParams->count--;
        if (sendParams->count == 0) {
          ESP_LOGD(consts::TAG, "SEND DONE");
          deinit(sendParams);
          vTaskDelete(NULL);
        }
      }

      /* Delay a while before sending the next data. */
      if (sendParams->delay > 0) {
        vTaskDelay(sendParams->delay / portTICK_RATE_MS);
      }

      ESP_LOGD(consts::TAG, "send data to " MACSTR "",
               MAC2STR(sendCallback->macAddress));

      memcpy(sendParams->destinationMac, sendCallback->macAddress,
             ESP_NOW_ETH_ALEN);
      prepareData(sendParams);

      /* Send the next data after the previous data is sent. */
      if (esp_now_send(sendParams->destinationMac, sendParams->buffer,
                       sendParams->len) != ESP_OK) {
        ESP_LOGE(consts::TAG, "SEND ERROR");
        deinit(sendParams);
        vTaskDelete(NULL);
      }
      break;
    }
    case structs::espNowEventID::espNowReceiveCallback: {
      structs::espNowEventReceiveCallback *receiveCallback =
          &event.info.receiveCallback;

      ret = parseData(
          reinterpret_cast<structs::espNowData *>(receiveCallback->data),
          receiveCallback->dataLength, &recv_state, &recv_seq, &recv_magic);
      delete receiveCallback->data;
      if (ret == structs::espNowCommunicationType::ESPNOW_DATA_BROADCAST) {
        ESP_LOGD(consts::TAG,
                 "RECEIVE %dthBROADCAST DATA FROM: " MACSTR ", LEN: %d",
                 recv_seq, MAC2STR(receiveCallback->macAddress),
                 receiveCallback->dataLength);

        /* If MAC address does not exist in peer list, add it to peer list. */
        if (esp_now_is_peer_exist(receiveCallback->macAddress) == false) {
          addPeer(receiveCallback->macAddress, sendParams);
        }

        /* Indicates that the device has received broadcast ESPNOW data. */
        if (sendParams->state == 0) {
          sendParams->state = 1;
        }

        /* If receive broadcast ESPNOW data which indicates that the other
         * device has received broadcast ESPNOW data and the local magic
         * number is bigger than that in the received broadcast ESPNOW data,
         * stop sending broadcast ESPNOW data and start sending unicast ESPNOW
         * data.
         */
        if (recv_state == 1) {
          /* The device which has the bigger magic number sends ESPNOW data,
           * the other one receives ESPNOW data.
           */
          if (sendParams->unicast == false && sendParams->magic >= recv_magic) {
            ESP_LOGD(consts::TAG, "START SENDING UNICAST DATA");
            ESP_LOGD(consts::TAG, "SEND DATA TO " MACSTR "",
                     MAC2STR(receiveCallback->macAddress));

            /* Start sending unicast ESPNOW data. */
            memcpy(sendParams->destinationMac, receiveCallback->macAddress,
                   ESP_NOW_ETH_ALEN);
            prepareData(sendParams);
            if (esp_now_send(sendParams->destinationMac, sendParams->buffer,
                             sendParams->len) != ESP_OK) {
              ESP_LOGE(consts::TAG, "Send error");
              deinit(sendParams);
              vTaskDelete(NULL);
            } else {
              sendParams->broadcast = false;
              sendParams->unicast = true;
            }
          }
        }
      } else if (ret == structs::espNowCommunicationType::ESPNOW_DATA_UNICAST) {
        ESP_LOGD(consts::TAG,
                 "RECEIVE %dth UNICAST DATA FROM: " MACSTR ", LEN: %d",
                 recv_seq, MAC2STR(receiveCallback->macAddress),
                 receiveCallback->dataLength);

        /* If receive unicast ESPNOW data, also stop sending broadcast ESPNOW
         * data. */
        sendParams->broadcast = false;
      } else {
        ESP_LOGD(consts::TAG, "RECEIVE ERROR DATA FROM: " MACSTR "",
                 MAC2STR(receiveCallback->macAddress));
      }
      break;
    }
    default:
      ESP_LOGE(consts::TAG, "CALLBACK TYPE ERROR: %d",
               static_cast<int>(event.id));
      break;
    }
  }
}

void EspnowDriver::addPeer(const uint8_t *macAddress,
                           structs::espNowParams *params) {
  esp_now_peer_info_t peer;
  peer.channel = consts::ESPNOW_CHANNEL;
  peer.ifidx = static_cast<wifi_interface_t>(ESPNOW_WIFI_IF);
  peer.encrypt = true;
  memcpy((&peer)->lmk, consts::ESPNOW_LMK, ESP_NOW_KEY_LEN);
  memcpy((&peer)->peer_addr, macAddress, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
  ESP_LOGE(consts::TAG, "ADD PEER SUCCESS");
}
void EspnowDriver::addBroadcastPeer() {
  /* Add broadcast peer information to peer list. */
  esp_now_peer_info_t peer;
  peer.channel = consts::ESPNOW_CHANNEL;
  peer.ifidx = static_cast<wifi_interface_t>(ESPNOW_WIFI_IF);
  peer.encrypt = false;
  memcpy((&peer)->peer_addr, consts::EXAMPLE_BROADCAST_MAC, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
}

structs::espNowParams *EspnowDriver::initializeSendingParameters(
    const bool unicast, const uint8_t state, const uint8_t destinationMac[],
    const uint32_t magic, const uint16_t count, const uint16_t delay,
    const int len) {
  structs::espNowParams *sendParam = new structs::espNowParams;
  if (sendParam == NULL) {
    ESP_LOGE(consts::TAG, "MALLOC SEND PARAMETER FAILURE");
    vSemaphoreDelete(ESPNOWqueue_);
    esp_now_deinit();
    return nullptr;
  }
  memset(sendParam, 0, sizeof(structs::espNowParams));

  if (unicast == true) {
    sendParam->unicast = true;
    sendParam->broadcast = false;
  } else {
    sendParam->unicast = false;
    sendParam->broadcast = true;
  }
  sendParam->state = state;
  sendParam->magic = magic;
  sendParam->count = count;
  sendParam->delay = delay;
  sendParam->len = len;
  sendParam->buffer = new uint8_t[consts::ESPNOW_SEND_LEN];
  if (sendParam->buffer == NULL) {
    ESP_LOGE(consts::TAG, "MALLOC SEND BUFFER FAIL");
    delete sendParam;
    vSemaphoreDelete(ESPNOWqueue_);
    esp_now_deinit();
    return nullptr;
  }
  memcpy(sendParam->destinationMac, destinationMac, ESP_NOW_ETH_ALEN);
  return sendParam;
}

} // namespace EspNow

} // namespace tamagotchi

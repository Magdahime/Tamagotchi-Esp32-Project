#include "ESPNOWmodule.hpp"
namespace tamagotchi {
namespace EspNowModule {

  
int init() {
  wifiInit();
  ESPNOWqueue = xQueueCreate(consts::ESPNOW_QUEUE_SIZE, sizeof(espNowEvent));
  if (ESPNOWqueue == NULL) {
    ESP_LOGE(consts::TAG, "CREATE MUTEX FAIL");
    return ESP_FAIL;
  }

  /* Initialize ESPNOW and register sending and receiving callback function. */
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_send_cb(sendData));
  ESP_ERROR_CHECK(esp_now_register_recv_cb(receiveData));

  /* Set primary master key. */
  ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)CONFIG_ESPNOW_PMK));

  /* Add broadcast peer information to peer list. */
  esp_now_peer_info_t peer;
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  (&peer)->channel = CONFIG_ESPNOW_CHANNEL;
  (&peer)->ifidx = static_cast<wifi_interface_t>(ESPNOW_WIFI_IF);
  (&peer)->encrypt = false;
  memcpy((&peer)->peer_addr, consts::EXAMPLE_BROADCAST_MAC, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));

  /* Initialize sending parameters. */
  espNowParams *sendParam = new espNowParams;
  memset(sendParam, 0, sizeof(espNowParams));
  if (sendParam == NULL) {
    ESP_LOGE(consts::TAG, "MALLOC SEND PARAMETER FAILURE");
    vSemaphoreDelete(ESPNOWqueue);
    esp_now_deinit();
    return ESP_FAIL;
  }
  sendParam->unicast = false;
  sendParam->broadcast = true;
  sendParam->state = 0;
  sendParam->magic = esp_random();
  sendParam->count = CONFIG_ESPNOW_SEND_COUNT;
  sendParam->delay = CONFIG_ESPNOW_SEND_DELAY;
  sendParam->len = CONFIG_ESPNOW_SEND_LEN;
  sendParam->buffer = new uint8_t[CONFIG_ESPNOW_SEND_LEN];
  if (sendParam->buffer == NULL) {
    ESP_LOGE(consts::TAG, "MALLOC SEND BUFFER FAIL");
    delete sendParam;
    vSemaphoreDelete(ESPNOWqueue);
    esp_now_deinit();
    return ESP_FAIL;
  }
  memcpy(sendParam->destinationMac, consts::EXAMPLE_BROADCAST_MAC,
         ESP_NOW_ETH_ALEN);
  prepareData(sendParam);

  xTaskCreate(ESPNOWtask, "espnowTask", 2048, sendParam, 4, NULL);
  return ESP_OK;
}

void deinit(espNowParams *params) {
  free(params->buffer);
  free(params);
  vSemaphoreDelete(ESPNOWqueue);
  esp_now_deinit();
}

void wifiInit() {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
  ESP_ERROR_CHECK(esp_wifi_start());

#if CONFIG_ESPNOW_ENABLE_LONG_RANGE
  ESP_ERROR_CHECK(esp_wifi_set_protocol(
      ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G |
                          WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
#endif
}

void sendData(const uint8_t *macAddress, esp_now_send_status_t status) {
  espNowEvent event;
  espNowEventSendCallback *sendCallback = &event.info.sendCallback;

  if (macAddress == NULL) {
    ESP_LOGE(consts::TAG, "SEND CALLBACK ERROR - MAC ADDRESS NULL");
    return;
  }

  event.id = espNowEventID::espNowSendCallback;
  memcpy(sendCallback->macAddress, macAddress, ESP_NOW_ETH_ALEN);
  sendCallback->status = status;
  if (xQueueSend(ESPNOWqueue, &event, consts::ESPNOW_MAXDELAY) != pdTRUE) {
    ESP_LOGW(consts::TAG, "SEND QUEUE FAIL");
  }
}

void receiveData(const uint8_t *macAddress, const uint8_t *data,
                 const int length) {
  espNowEvent event;
  espNowEventReceiveCallback *receiveCallback = &event.info.receiveCallback;

  if (macAddress == NULL || data == NULL || length <= 0) {
    ESP_LOGE(consts::TAG, "RECEIVE CALLBACK ARGUMENT ERROR");
    return;
  }
  event.id = espNowEventID::espNowReceiveCallback;
  memcpy(receiveCallback->macAddress, macAddress, ESP_NOW_ETH_ALEN);
  receiveCallback->data = new uint8_t[length];
  if (receiveCallback->data == NULL) {
    ESP_LOGE(consts::TAG, "MALLOC RECEIVE DATA FAILURE");
    return;
  }

  memcpy(receiveCallback->data, data, length);
  receiveCallback->dataLength = length;
  if (xQueueSend(ESPNOWqueue, &event, consts::ESPNOW_MAXDELAY) != pdTRUE) {
    ESP_LOGW(consts::TAG, "RECEIVE QUEUE FAIL");
    delete[] receiveCallback->data;
  }
}

espNowCommunicationType parseData(espNowData *data, uint16_t dataLength,
                                  uint8_t *state, uint16_t *sequence,
                                  int *magic) {

  uint16_t crc, crc_cal = 0;

  if (dataLength < sizeof(espNowData)) {
    ESP_LOGE(consts::TAG, "RECEIVED ESP NOW DATA IS TOO SHORT, len:%d",
             dataLength);
    return espNowCommunicationType::ESPNOW_ERROR;
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
  return espNowCommunicationType::ESPNOW_ERROR;
}

void prepareData(espNowParams *sendParams) {
  espNowData *buf = (espNowData *)sendParams->buffer;
  assert(sendParams->len >= sizeof(espNowData));
  buf->type = isBroadcastAddress(sendParams->destinationMac)
                  ? espNowCommunicationType::ESPNOW_DATA_BROADCAST
                  : espNowCommunicationType::ESPNOW_DATA_UNICAST;
  buf->state = sendParams->state;
  buf->sequenceNumber = messageCounter[static_cast<int>(buf->type)]++;
  buf->crc = 0;
  buf->magic = sendParams->magic;
  /* Fill all remaining bytes after the data with random values */
  esp_fill_random(buf->payload, sendParams->len - sizeof(espNowData));
  buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, sendParams->len);
}

void ESPNOWtask(void *pvParameter) {

  espNowEvent event;
  uint8_t recv_state = 0;
  uint16_t recv_seq = 0;
  int recv_magic = 0;
  bool is_broadcast = false;
  espNowCommunicationType ret;

  vTaskDelay(5000 / portTICK_RATE_MS);
  ESP_LOGI(consts::TAG, "Start sending broadcast data");

  /* Start sending broadcast ESPNOW data. */
  espNowParams *sendParams = (espNowParams *)pvParameter;
  if (esp_now_send(sendParams->destinationMac, sendParams->buffer,
                   sendParams->len) != ESP_OK) {
    ESP_LOGE(consts::TAG, "ESPNOW SEND ERROR");
    deinit(sendParams);
    vTaskDelete(NULL);
  }

  while (xQueueReceive(ESPNOWqueue, &event, portMAX_DELAY) == pdTRUE) {
    switch (event.id) {
    case espNowEventID::espNowSendCallback: {
      espNowEventSendCallback *sendCallback = &event.info.sendCallback;
      is_broadcast = isBroadcastAddress(sendCallback->macAddress);

      ESP_LOGD(consts::TAG, "Send data to " MACSTR ", status1: %d",
               MAC2STR(sendCallback->macAddress), sendCallback->status);

      if (is_broadcast && (sendParams->broadcast == false)) {
        break;
      }

      if (!is_broadcast) {
        sendParams->count--;
        if (sendParams->count == 0) {
          ESP_LOGI(consts::TAG, "SEND DONE");
          deinit(sendParams);
          vTaskDelete(NULL);
        }
      }

      /* Delay a while before sending the next data. */
      if (sendParams->delay > 0) {
        vTaskDelay(sendParams->delay / portTICK_RATE_MS);
      }

      ESP_LOGI(consts::TAG, "send data to " MACSTR "",
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
    case espNowEventID::espNowReceiveCallback: {
      espNowEventReceiveCallback *receiveCallback = &event.info.receiveCallback;

      ret = parseData(reinterpret_cast<espNowData *>(receiveCallback->data),
                      receiveCallback->dataLength, &recv_state, &recv_seq,
                      &recv_magic);
      free(receiveCallback->data);
      if (ret == espNowCommunicationType::ESPNOW_DATA_BROADCAST) {
        ESP_LOGI(consts::TAG,
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
         * device has received broadcast ESPNOW data and the local magic number
         * is bigger than that in the received broadcast ESPNOW data, stop
         * sending broadcast ESPNOW data and start sending unicast ESPNOW data.
         */
        if (recv_state == 1) {
          /* The device which has the bigger magic number sends ESPNOW data, the
           * other one receives ESPNOW data.
           */
          if (sendParams->unicast == false && sendParams->magic >= recv_magic) {
            ESP_LOGI(consts::TAG, "START SENDING UNICAST DATA");
            ESP_LOGI(consts::TAG, "SEND DATA TO " MACSTR "",
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
      } else if (ret == espNowCommunicationType::ESPNOW_DATA_UNICAST) {
        ESP_LOGI(consts::TAG,
                 "RECEIVE %dth UNICAST DATA FROM: " MACSTR ", LEN: %d",
                 recv_seq, MAC2STR(receiveCallback->macAddress),
                 receiveCallback->dataLength);

        /* If receive unicast ESPNOW data, also stop sending broadcast ESPNOW
         * data. */
        sendParams->broadcast = false;
      } else {
        ESP_LOGI(consts::TAG, "RECEIVE ERROR DATA FROM: " MACSTR "",
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

void addPeer(const uint8_t *macAddress, espNowParams *params) {
  esp_now_peer_info_t peer;
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  (&peer)->channel = CONFIG_ESPNOW_CHANNEL;
  (&peer)->ifidx = static_cast<wifi_interface_t>(ESPNOW_WIFI_IF);
  (&peer)->encrypt = true;
  memcpy((&peer)->lmk, CONFIG_ESPNOW_LMK, ESP_NOW_KEY_LEN);
  memcpy((&peer)->peer_addr, macAddress, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
  ESP_LOGE(consts::TAG, "ADD PEER SUCCESS");
}
} // namespace EspNowModule

} // namespace tamagotchi

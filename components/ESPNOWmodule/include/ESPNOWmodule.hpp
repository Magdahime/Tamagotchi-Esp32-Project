#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ESPNOWspec.hpp"
#include "esp_crc.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_now.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "nvs_flash.h"

namespace tamagotchi {
class ESPNOWModule {
 public:
  static int init();
  static void deinit(espNowParams *params);
  static void sendData(const uint8_t *macAddress, esp_now_send_status_t status);
  static void receiveData(const uint8_t *macAddress, const uint8_t *data,
                          const int length);
  static espNowCommunicationType parseData(espNowData *data,
                                           uint16_t dataLength, uint8_t *state,
                                           uint16_t *sequence, int *magic);
  static void prepareData(espNowParams *sendParams);
  static void ESPNOWtask(void *pvParameter);
  static espNowParams * initializeSendingParameters(const bool unicast, const uint8_t state,
                                          const uint8_t destinationMac[],
                                          const uint32_t magic =  esp_random(),
                                          const uint16_t count = consts::ESPNOW_SEND_COUNT,
                                          const uint16_t delay = consts::ESPNOW_SEND_DELAY,
                                          const int len = consts::ESPNOW_SEND_LEN);

 private:
  static void wifiInit();
  static void addBroadcastPeer();
  static void addPeer(const uint8_t *macAddress, espNowParams *params);
  static xQueueHandle ESPNOWqueue;
  static int64_t messageCounter[consts::ESPNOW_COMMUNICATION_METHODS];
};  // namespace EspNowModule
}  // namespace tamagotchi

#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ESPNOWconf.hpp"
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
namespace EspNow {
class EspnowDriver {
public:
  static int init();
  static void deinit(structs::espNowParams *params);
  static void sendData(const uint8_t *macAddress, esp_now_send_status_t status);
  static void receiveData(const uint8_t *macAddress, const uint8_t *data,
                          const int length);
  static structs::espNowCommunicationType parseData(structs::espNowData *data,
                                           uint16_t dataLength, uint8_t *state,
                                           uint16_t *sequence, int *magic);
  static void prepareData(structs::espNowParams *sendParams);
  static void ESPNOWtask(void *pvParameter);
  static structs::espNowParams *
  initializeSendingParameters(const bool unicast, const uint8_t state,
                              const uint8_t destinationMac[],
                              const uint32_t magic = esp_random(),
                              const uint16_t count = consts::ESPNOW_SEND_COUNT,
                              const uint16_t delay = consts::ESPNOW_SEND_DELAY,
                              const int len = consts::ESPNOW_SEND_LEN);

private:
  static void wifiInit();
  static void addBroadcastPeer();
  static void addPeer(const uint8_t *macAddress, structs::espNowParams *params);
  static xQueueHandle ESPNOWqueue_;
  static int64_t messageCounter_[consts::ESPNOW_COMMUNICATION_METHODS];
};

} // namespace EspNow
} // namespace tamagotchi

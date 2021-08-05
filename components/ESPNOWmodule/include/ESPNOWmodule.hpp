#pragma once

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
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace tamagotchi {
namespace EspNowModule {
int init();
void deinit(espNowParams *params);
void sendData(const uint8_t *macAddress, esp_now_send_status_t status);
void receiveData(const uint8_t *macAddress, const uint8_t *data,
                 const int length);
espNowCommunicationType parseData(espNowData *data, uint16_t dataLength,
                                  uint8_t *state, uint16_t *sequence,
                                  int *magic);
void prepareData(espNowParams *sendParams);
void ESPNOWtask(void *pvParameter);

void wifiInit();
void addPeer(const uint8_t *macAddress, espNowParams *params);
static xQueueHandle ESPNOWqueue;
static int64_t messageCounter[consts::ESPNOW_COMMUNICATION_METHODS] = {0, 0};
}; // namespace EspNowModule
} // namespace tamagotchi


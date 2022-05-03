#pragma once

#include <stdint.h>

#include <cstring>

#include "GomokuNetworkingConf.hpp"
#include "MessageQueue.hpp"
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

namespace App {

namespace Gomoku {

class GomokuNetworking {
 public:
  static void init();
  static void run();
  static void sendData(const uint8_t *macAddress, esp_now_send_status_t status);
  static void receiveData(const uint8_t *macAddress, const uint8_t *data,
                          const int length);
  static SemaphoreHandle_t &mutex() { return mutex_; }
  static void deinit();

 private:
  static void task(void *pvParameters);
  static TaskHandle_t gomokuNetworkingTask;
  static structs::GomokuParams sendParams_;
  static uint8_t hostAddress_[ESP_NOW_ETH_ALEN];
  static constexpr char TAG_[] = "GomokuNetworking";
  static SemaphoreHandle_t mutex_;
  static MessageQueue::MessageQueue<structs::GomokuEvent> gomokuQueue_;
  static void searchForFriends();
  static structs::receiveCallbackSummary parseData(structs::GomokuData *data,
                                                   int dataLength);
  static void sendGameInvite(structs::GomokuParams &sendParams);
  static void addPeer(const uint8_t *macAddress);
};

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi

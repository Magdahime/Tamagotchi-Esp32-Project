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
  GomokuNetworking();
  void searchForFriends();

 private:
  int players_;
  structs::GomokuParams sendParams_;
  uint8_t hostAddress_[ESP_NOW_ETH_ALEN];
  static constexpr char TAG_[] = "GomokuNetworking";
  static MessageQueue::MessageQueue<structs::GomokuEvent> gomokuQueue_;
  
  structs::receiveCallbackSummary parseData(structs::GomokuData *data,
                                            int dataLength);
  void sendGameInvite(structs::GomokuParams &sendParams);
  void addPeer(const uint8_t *macAddress);
  static void sendData(const uint8_t *macAddress, esp_now_send_status_t status);
  static void receiveData(const uint8_t *macAddress, const uint8_t *data,
                          const int length);
};

}  // namespace Gomoku
}  // namespace App
}  // namespace tamagotchi

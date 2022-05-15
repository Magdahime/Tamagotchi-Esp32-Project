#pragma once

#include <stdint.h>

#include <cstring>
#include <vector>

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

namespace tamagotchi::App::Gomoku {

class GomokuNetworking {
 public:
  static void init();
  static TaskHandle_t &run();
  static void sendData(const uint8_t *macAddress, esp_now_send_status_t status);
  static void receiveData(const uint8_t *macAddress, const uint8_t *data,
                          const int length);

  static mac_address_t &gameHostAddress() { return gameHostAddress_; }
  static mac_address_t &hostAddress() { return hostAddress_; }
  static std::vector<mac_address_t> &playersMacs() { return playersMacs_; }
  static MessageQueue::MessageQueue<structs::GomokuEvent> gomokuQueue() {
    return gomokuQueue_;
  }
  static MessageQueue::MessageQueue<structs::GomokuDataWithRecipient>
  sendingQueue() {
    return sendingQueue_;
  }
  static TaskHandle_t &gomokuNetworkingTask() { return gomokuNetworkingTask_; }

  static void deinit();

 private:
  static void task(void *pvParameters);
  static TaskHandle_t gomokuNetworkingTask_;
  static structs::GomokuParams sendParams_;
  static mac_address_t gameHostAddress_;
  static mac_address_t hostAddress_;
  static constexpr char TAG_[] = "GomokuNetworking";
  static std::vector<mac_address_t> playersMacs_;
  static MessageQueue::MessageQueue<structs::GomokuEvent> gomokuQueue_;
  static MessageQueue::MessageQueue<structs::GomokuDataWithRecipient>
      sendingQueue_;

  static void searchForFriends();
  static void handleCommunication();

  static structs::ReceiveCallbackSummary parseData(structs::GomokuData data);
  static void sendGameInvite();

  static void chooseHost(mac_address_t &peer,
                         structs::ReceiveCallbackSummary &summary);
  static bool addPeer(mac_address_t &peer,
                      structs::ReceiveCallbackSummary &summary, int players);
  static void addPeerESP(const uint8_t *macAddress);
  static void prepareData();
};

}  // namespace tamagotchi::App::Gomoku

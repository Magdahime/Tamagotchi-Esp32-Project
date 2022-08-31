#pragma once

#include <stdint.h>

#include <cstring>
#include <list>

#include "BitmapPet.hpp"
#include "GomokuNetworkingConf.hpp"
#include "MessageQueue.hpp"
#include "PetGenerator.hpp"
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

namespace tamagotchi::App::GomokuNetworking {

class GomokuNetworking {
 public:
  static void init();
  static TaskHandle_t &run();
  static void sendDataCallback(const uint8_t *macAddress,
                               esp_now_send_status_t status);
  static void receiveDataCallback(const uint8_t *macAddress,
                                  const uint8_t *data, const int length);
  static void sendMessage(structs::GomokuDataWithRecipient message);
  static structs::GomokuData unpackData(structs::GomokuEvent event);
  static mac_address_t gameHostAddress() { return gameHostAddress_; }
  static mac_address_t hostAddress() { return hostAddress_; }
  static std::vector<mac_address_t> playersMacs();
  static structs::HostParams &hostParams() { return hostParams_; }
  static MessageQueue::MessageQueue<structs::GomokuEvent> receiveQueue() {
    return receiveQueue_;
  }
  static MessageQueue::MessageQueue<structs::GomokuDataWithRecipient>
  sendingQueue() {
    return sendingQueue_;
  }
  static MessageQueue::MessageQueue<structs::GomokuEvent> hostQueue() {
    return gameQueue_;
  }

  static TaskHandle_t &gomokuNetworkingTask() { return gomokuNetworkingTask_; }
  static std::vector<std::pair<mac_address_t, Pet::BitmapPet<uint16_t>>>
      &playersParams() {
    return playersParams_;
  }

  static void setDeinit() { ifDeinit_ = true; }
  static void deinit();

 private:
  static bool ifDeinit_;
  static TaskHandle_t gomokuNetworkingTask_;
  static structs::GomokuParams sendParams_;
  static structs::HostParams hostParams_;
  static mac_address_t gameHostAddress_;
  static mac_address_t hostAddress_;
  static constexpr char TAG_[] = "GomokuNetworking";
  static std::vector<std::pair<mac_address_t, Pet::BitmapPet<uint16_t>>>
      playersParams_;

  static MessageQueue::MessageQueue<structs::GomokuEvent> receiveQueue_;
  static MessageQueue::MessageQueue<structs::GomokuDataWithRecipient>
      sendingQueue_;
  static MessageQueue::MessageQueue<structs::GomokuEvent> gameQueue_;

  static void task(void *pvParameters);
  static void searchForFriends();
  static void handleCommunicationHost();
  static void handleCommunicationPlayer();
  static void sendGameInvite();
  static void retransmit(
      std::vector<structs::SenderParams> &sendersLiveParams,
      std::list<std::pair<uint32_t, structs::GomokuDataWithRecipient>>
          &ackMessageQueue);
  static void collectACKFromHost(structs::GomokuDataWithRecipient sendMsg);
  static void startCollectingACKs();
  static void handleMessage(
      structs::GomokuEvent &message,
      std::vector<structs::SenderParams> &sendersLiveParams,
      std::list<std::pair<uint32_t, structs::GomokuDataWithRecipient>>
          &ackMessageQueue);
  static void chooseHost(mac_address_t &peer, structs::GomokuData &data);
  static bool addPeer(Pet::PetGenerator<uint16_t> &petGenerator,
                      mac_address_t &peer, structs::GomokuData &data,
                      int players);
  static void addPeerESP(const uint8_t *macAddress);
  static void prepareData();

  static void removeFromAckMessageQueue(
      uint32_t id,
      std::list<std::pair<uint32_t, structs::GomokuDataWithRecipient>>
          &ackMessageQueue);
};

}  // namespace tamagotchi::App::GomokuNetworking

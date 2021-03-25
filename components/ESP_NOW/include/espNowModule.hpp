#ifndef ESP_NOW_MODULE
#define ESP_NOW_MODULE

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"
#include "tcpip_adapter.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_now.h"
#include "rom/ets_sys.h"
#include "rom/crc.h"
#include "espNowStructs.hpp"
#include "wifiModule.hpp"

class EspNowModule {
public:
    static EspNowModule& getInstance();
    void init();
    void deinit(espNowParams * params);
    void addPeer();
    void deletePeer();
    void sendData(const uint8_t* macAddress, esp_now_send_status_t status);
    void receiveData();
    void parseData();
    void setRole();

private:
};

#endif
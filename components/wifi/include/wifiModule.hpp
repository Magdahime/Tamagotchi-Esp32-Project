#pragma once
#include <string>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "semphr.h"
#include "esp_now.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "espNowConfigurations"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

class WifiModule {

    public:
    enum class WifiMode { ESP_NOW, STATION, ACCESS_POINT };
    static void wifiInit(WifiMode mode);
    static void wifiConnect(std::string wifiSSID, std::string wifiPassword);
    static void wifiDeinit();

    private:
    WifiModule(std::string wifiSSID, std::string wifiPassword) : wifiSSID(wifiSSID), 
                                                                wifiPassword(wifiPassword)
                                                                {}
    void connect();
    static void eventHandler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);
    
    static SemaphoreHandle_t wifiMutex; 
    static int maximumRetry;
    static bool ifWifiInit;
    static EventGroupHandle_t wifiEventGroup;
    static std::string TAG;
    
    const std::string wifiSSID;
    const std::string wifiPassword;
};
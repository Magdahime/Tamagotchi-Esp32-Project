#ifndef WIFI_MODULE
#define WIFI_MODULE

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
#include "espNowStructs.hpp"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

class WifiModule {

    public:
    enum class WifiMode { ESP_NOW, STATION, ACCESS_POINT };
    static WifiModule* getInstance();
    void wifiInit(WifiMode mode);
    void wifiConnect(std::string wifiSSID, std::string wifiPassword);
    void wifiDeinit();
    void operator=(WifiModule const&) = delete;

    private:
    WifiModule();
    void connect();
    static void eventHandler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);
    
    void setPassword(std::string wifiPassword);
    void setSSID(std::string wifiSSID);
    static const int maximumRetry;
    static EventGroupHandle_t wifiEventGroup;
    static std::string TAG;
    static WifiModule* instance;

    bool ifWifiInit;
    SemaphoreHandle_t wifiMutex; 
    std::string wifiSSID;
    std::string wifiPassword;
};

#endif
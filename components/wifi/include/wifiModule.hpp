#include <string>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

class WifiModule {
    public:

    static void wifiConnect(std::string wifiSSID, std::string wifiPassword);
    
    private:
    WifiModule(std::string wifiSSID, std::string wifiPassword) : wifiSSID(wifiSSID), 
                                                                wifiPassword(wifiPassword)
                                                                {}
    void connect();
    static void eventHandler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);
    static int maximumRetry;
    const std::string wifiSSID;
    const std::string wifiPassword;
    static EventGroupHandle_t wifiEventGroup;
    static std::string TAG;
};
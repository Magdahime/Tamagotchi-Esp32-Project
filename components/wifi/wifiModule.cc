#include "wifiModule.hpp"

int WifiModule::maximumRetry = 3;
std::string WifiModule::TAG = "ESP wifi station";
EventGroupHandle_t WifiModule::wifiEventGroup = xEventGroupCreate();

void WifiModule::wifiConnect(std::string wifiSSID, std::string wifiPassword){
    WifiModule instance(wifiSSID, wifiPassword);
    instance.connect();
}

void WifiModule::eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    int s_retry_num = 0;

      if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < maximumRetry) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG.c_str(), "retry to connect to the AP");
        } else {
            xEventGroupSetBits(wifiEventGroup, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG.c_str(),"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG.c_str(), "got ip:%s",
                 ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(wifiEventGroup, WIFI_CONNECTED_BIT);
    }                       
}

void WifiModule::connect(){

    wifiEventGroup = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiModule::eventHandler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WifiModule::eventHandler, NULL));


    wifi_config_t wifi_config = {};
    strcpy((char *)wifi_config.sta.ssid, wifiSSID.c_str());
    strcpy((char *)wifi_config.sta.password, wifiPassword.c_str());

    if (strlen((char *)wifi_config.sta.password)) {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG.c_str(), "wifi_init_sta finished.");

    EventBits_t bits = xEventGroupWaitBits(wifiEventGroup,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG.c_str(), "connected to ap SSID:%s password:%s",
                wifiSSID.c_str(), wifiPassword.c_str());
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG.c_str(), "Failed to connect to SSID:%s, password:%s",
                 wifiSSID.c_str(), wifiPassword.c_str());
    } else {
        ESP_LOGE(TAG.c_str(), "UNEXPECTED EVENT");
    }

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &WifiModule::eventHandler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiModule::eventHandler));
    vEventGroupDelete(wifiEventGroup);
}
#include "espNowModule.hpp"
#include "wifiModule.hpp"


EspNowModule& getInstance(){
    static EspNowModule instance;
    return instance;
}

void EspNowModule::init(){
    WifiModule::getInstance()->wifiInit(WifiModule::WifiMode::ESP_NOW);
}

void EspNowModule::deinit(espNowParams * params){
    // free(send_param->buffer);
    // free(send_param);
    // vSemaphoreDelete(example_espnow_queue);
    // esp_now_deinit();
    WifiModule::getInstance()->wifiDeinit();
}

void EspNowModule::sendData(const uint8_t* macAddress, esp_now_send_status_t status){
    espNowEvent event;
    espNowEventSendCallback * sendCallback = &event.info.sendCallback;
    if (macAddress == NULL) {
        printf("SEND CALLBACK ERROR - MAC ADDRESS NULL");
        return;
    }

    event.id = espNowEventID::espNowSendCallback;
    
    
}
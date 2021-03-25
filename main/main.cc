#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "wifiModule.hpp"


extern "C" void app_main()
    {
        ESP_ERROR_CHECK(nvs_flash_init());
        printf("Hello tamagotchi!\n");
        WifiModule::getInstance()->wifiConnect("UPC121667", "pGx63cPyysu3");
    }

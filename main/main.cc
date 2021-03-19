#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "wifiModule.hpp"


extern "C" void app_main()
    {
        printf("Hello tamagotchi!\n");

        ESP_ERROR_CHECK(nvs_flash_init());

    }

#include <cstdio>

#include "EspnowDriver.hpp"
#include "Joystick.hpp"
#include "ST7789Conf.hpp"
#include "ST7789Driver.hpp"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main() {
  printf("Hello tamagotchi!\n");
  // esp_log_level_set("*", ESP_LOG_DEBUG);
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
}

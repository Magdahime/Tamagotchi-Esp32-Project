#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cstdio>

#include "EspnowDriver.hpp"
#include "Joystick.hpp"
#include "ST7789Driver.hpp"

extern "C" void app_main() {
  printf("Hello tamagotchi!\n");
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  // tamagotchi::EspnowDriver::init();
  // tamagotchi::Joystick joystick;
  // joystick.init();

  tamagotchi::ST7789::structs::gpio_config_t gpio = {.gpioMosi = 13,
                                                     .gpioSclk = 14,
                                                     .gpioCs = 15,
                                                     .gpioDc = 2,
                                                     .gpioReset = 4,
                                                     .gpioBl = -1};

  tamagotchi::ST7789::structs::lcd_config_t lcd{
      .width = 240, .height = 320, .offsetx = 0, .offsety = 0};

  tamagotchi::ST7789::structs::st7789_config_t config = {.gpio = gpio,
                                                         .lcd = lcd};

  tamagotchi::ST7789::ST7789VWDriver st7789Driver(config);
}

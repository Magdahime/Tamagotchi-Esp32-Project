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
  esp_log_level_set("*", ESP_LOG_DEBUG);
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  // tamagotchi::EspnowDriver::init();
  // tamagotchi::Joystick joystick;
  // joystick.init();

  tamagotchi::ST7789::structs::gpio_config_t gpio = {.gpioMosi = (gpio_num_t)13,
                                                     .gpioSclk = (gpio_num_t)14,
                                                     .gpioCs = (gpio_num_t)15,
                                                     .gpioDc = (gpio_num_t)2,
                                                     .gpioReset = (gpio_num_t)4,
                                                     .gpioBl = (gpio_num_t)-1};

  tamagotchi::ST7789::structs::lcd_config_t lcd{
      .width = 240, .height = 320, .offsetx = 0, .offsety = 0};

  tamagotchi::ST7789::structs::st7789_config_t config = {.gpio = gpio,
                                                         .lcd = lcd};

  tamagotchi::ST7789::ST7789VWDriver st7789Driver(config);
  st7789Driver.fillScreen(tamagotchi::ST7789::colours::RED);
  st7789Driver.drawLine(0, 0, 239, 160, tamagotchi::ST7789::colours::BLACK);
  st7789Driver.drawLine(0, 160, 239, 0, tamagotchi::ST7789::colours::BLACK);
}

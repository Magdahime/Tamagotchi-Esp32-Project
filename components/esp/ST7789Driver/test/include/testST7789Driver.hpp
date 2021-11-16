#pragma once
#include "ST7789Driver.hpp"
//#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


tamagotchi::ST7789::ST7789VWDriver initializeDisplay() {
  tamagotchi::ST7789::structs::gpio_config_t gpio = {.gpioMosi = (gpio_num_t)13,
                                         .gpioSclk = (gpio_num_t)14,
                                         .gpioCs = (gpio_num_t)15,
                                         .gpioDc = (gpio_num_t)2,
                                         .gpioReset = (gpio_num_t)4,
                                         .gpioBl = (gpio_num_t)-1};

  tamagotchi::ST7789::structs::lcd_config_t lcd{
      .width = 240, .height = 320, .offsetx = 0, .offsety = 0};

  tamagotchi::ST7789::structs::st7789_config_t config = {.gpio = gpio, .lcd = lcd};

  return tamagotchi::ST7789::ST7789VWDriver(config);
}
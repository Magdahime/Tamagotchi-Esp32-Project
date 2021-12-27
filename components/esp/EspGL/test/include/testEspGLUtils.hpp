#pragma once
#include "ST7789Driver.hpp"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace tamagotchi::ST7789;
using namespace tamagotchi::EspGL;

tamagotchi::ST7789::ST7789VWDriver initializeDisplay() {
  structs::gpio_config_t gpio = {.gpioMosi = (gpio_num_t)consts::GPIOMOSI,
                                 .gpioSclk = (gpio_num_t)consts::GPIOSCLK,
                                 .gpioCs = (gpio_num_t)consts::GPIOCS,
                                 .gpioDc = (gpio_num_t)consts::GPIODC,
                                 .gpioReset = (gpio_num_t)consts::GPIORESET,
                                 .gpioBl = (gpio_num_t)consts::GPIOBL};

  structs::lcd_config_t lcd{.width = consts::WIDTH,
                            .height = consts::HEIGHT,
                            .offsetx = consts::OFFSETX,
                            .offsety = consts::OFFSETY};

  structs::st7789_config_t config = {.gpio = gpio, .lcd = lcd};
  return tamagotchi::ST7789::ST7789VWDriver(config);
}
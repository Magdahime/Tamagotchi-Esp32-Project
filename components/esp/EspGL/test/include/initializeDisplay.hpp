#pragma once
#include "ST7789Driver.hpp"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace tamagotchi::ST7789;
using namespace tamagotchi::EspGL;

tamagotchi::ST7789::ST7789VWDriver initializeDisplay();
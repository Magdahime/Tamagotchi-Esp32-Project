#pragma once

#include <unordered_map>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace tamagotchi {

namespace GPIOInputs {
constexpr gpio_num_t GPIO_RST = GPIO_NUM_27;
constexpr gpio_num_t GPIO_SET = GPIO_NUM_26;
constexpr gpio_num_t GPIO_UP = GPIO_NUM_22;
constexpr gpio_num_t GPIO_DOWN = GPIO_NUM_21;
constexpr gpio_num_t GPIO_LEFT = GPIO_NUM_32;
constexpr gpio_num_t GPIO_RIGHT = GPIO_NUM_33;
constexpr gpio_num_t GPIO_MIDDLE = GPIO_NUM_25;

static const std::unordered_map<gpio_num_t, const char*> gpio2string = {
    {GPIO_RST, "RESET"},    {GPIO_SET, "SET"},   {GPIO_UP, "UP"},
    {GPIO_DOWN, "DOWN"},    {GPIO_LEFT, "LEFT"}, {GPIO_RIGHT, "RIGHT"},
    {GPIO_MIDDLE, "MIDDLE"}

};

}  // namespace GPIOInputs

namespace GPIOConsts {
constexpr int DEBOUNCE_TIME = 10;
constexpr int SUCCESS_DELAY = 15 / portTICK_PERIOD_MS;
constexpr int FAILURE_DELAY = 10 / portTICK_PERIOD_MS;
constexpr int ESP_INTR_FLAG_DEFAULT = 0;
constexpr char TAG[] = "ESP32 GPIOModule";
}  // namespace GPIOConsts

enum class GPIOPullMode { PULLUP, PULLDOWN };
enum class GPIOIOmode { INPUT, OUTPUT };
enum class GPIOedge { ANY, RISING, FALLING };

}  // namespace tamagotchi

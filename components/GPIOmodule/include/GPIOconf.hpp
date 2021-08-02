#ifndef GPIOCONF
#define GPIOCONF
#include "driver/gpio.h"

namespace tamagotchi {

namespace GPIOInputs {
constexpr gpio_num_t GPIO_RST = GPIO_NUM_4;
constexpr gpio_num_t GPIO_SET = GPIO_NUM_5;
constexpr gpio_num_t GPIO_UP = GPIO_NUM_16;
constexpr gpio_num_t GPIO_DOWN = GPIO_NUM_17;
constexpr gpio_num_t GPIO_LEFT = GPIO_NUM_18;
constexpr gpio_num_t GPIO_RIGHT = GPIO_NUM_19;
constexpr gpio_num_t GPIO_MIDDLE = GPIO_NUM_21;

} // namespace GPIOInputs

namespace GPIOConsts {
constexpr int ESP_INTR_FLAG_DEFAULT = 0;
constexpr char TAG[] = "ESP32 GPIOModule";
}

enum class GPIOPullMode { PULLUP, PULLDOWN };
enum class GPIOIOmode { INPUT, OUTPUT };
enum class GPIOedge { ANY, RISING, FALLING };

} // namespace tamagotchi

#endif
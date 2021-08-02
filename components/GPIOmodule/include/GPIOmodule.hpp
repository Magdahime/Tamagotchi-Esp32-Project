#ifndef GPIOMODULE
#define GPIOMODULE

#include "GPIOconf.hpp"
#include "driver/gpio.h"
#include <esp_log.h>


namespace tamagotchi {
namespace GPIOModule {
void GPIOinit(gpio_num_t GPIOnum, GPIOPullMode pull, GPIOIOmode mode,
              GPIOedge edge);

void setHandler(gpio_num_t GPIOnum, gpio_isr_t isrHandler);
void removeHandler(gpio_num_t GPIOnum);
}; // namespace GPIOModule

} // namespace tamagotchi

#endif
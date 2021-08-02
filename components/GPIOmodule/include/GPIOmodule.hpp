#ifndef GPIOMODULE
#define GPIOMODULE

#include <esp_log.h>

#include <vector>

#include "GPIOconf.hpp"
#include "driver/gpio.h"
#include "hal/gpio_types.h"


namespace tamagotchi {
namespace GPIOModule {
void GPIOinit(const gpio_num_t GPIOnum, const GPIOPullMode pull,
              const GPIOIOmode mode, const GPIOedge edge);
void GPIOinit(const std::vector<gpio_num_t>& GPIOnum, const GPIOPullMode pull,
              const GPIOIOmode mode, const GPIOedge edge);
void setHandler(const gpio_num_t GPIOnum, const gpio_isr_t isrHandler);
void setHandler(const std::vector<gpio_num_t>& GPIOnum,
                const gpio_isr_t isrHandler);
void removeHandler(const gpio_num_t GPIOnum);
};  // namespace GPIOModule

}  // namespace tamagotchi

#endif
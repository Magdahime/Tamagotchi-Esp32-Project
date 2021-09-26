#pragma once

#include <esp_log.h>

#include <vector>

#include "GPIOconf.hpp"
#include "driver/gpio.h"
#include "hal/gpio_types.h"

namespace tamagotchi {
class GpioDriver {
 public:
  static void GPIOinit(const gpio_num_t GPIOnum, const GPIOPullMode pull,
                       const GPIOIOmode mode, const GPIOedge edge);
  static void GPIOinit(const std::vector<gpio_num_t>& GPIOnum,
                       const GPIOPullMode pull, const GPIOIOmode mode,
                       const GPIOedge edge);
  static void setHandler(const gpio_num_t GPIOnum, const gpio_isr_t isrHandler);
  static void setHandler(const std::vector<gpio_num_t>& GPIOnum,
                         const gpio_isr_t isrHandler);
  static void removeHandler(const gpio_num_t GPIOnum);

  static bool getLevel(const gpio_num_t GPIOnum);
  static void setLevel(const gpio_num_t GPIOnum, bool state);

 private:
  static const char* TAG_;
};

}  // namespace tamagotchi

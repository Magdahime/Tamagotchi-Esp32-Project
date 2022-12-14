#include "GpioDriver.hpp"

namespace tamagotchi {

namespace Gpio {

const char *GpioDriver::TAG_ = "ESP32 GpioDriver";

void GpioDriver::GPIOinit(const gpio_num_t GPIOnum, const GPIOPullMode pull,
                          const GPIOIOmode mode, const GPIOedge edge) {
  gpio_config_t gpioConfig;
  switch (edge) {
  case Gpio::GPIOedge::ANY:
    gpioConfig.intr_type = GPIO_INTR_ANYEDGE;
    break;
  case Gpio::GPIOedge::RISING:
    gpioConfig.intr_type = GPIO_INTR_POSEDGE;
    break;
  case Gpio::GPIOedge::FALLING:
    gpioConfig.intr_type = GPIO_INTR_NEGEDGE;
    break;
  }

  gpioConfig.mode =
      mode == Gpio::GPIOIOmode::INPUT ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT;
  gpioConfig.pin_bit_mask = (1ULL << GPIOnum);

  // SET PULL MODE
  if (pull == GPIOPullMode::PULLUP) {
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
  } else {
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
  }

  gpio_config(&gpioConfig);
  ESP_LOGI(TAG_, "GPIO[%d] configured", GPIOnum);
}

void GpioDriver::GPIOinit(const std::vector<gpio_num_t> &GPIOnum,
                          const GPIOPullMode pull, const GPIOIOmode mode,
                          const GPIOedge edge) {
  for (const auto &num : GPIOnum) {
    GPIOinit(num, pull, mode, edge);
  }
  ESP_LOGI(TAG_, "End of GPIOInit");
}

void GpioDriver::setHandler(const std::vector<gpio_num_t> &GPIOnum,
                            const gpio_isr_t isrHandler) {
  for (const auto &num : GPIOnum) {
    setHandler(num, isrHandler);
  }
  ESP_LOGI(TAG_, "End of setting handlers");
}

void GpioDriver::setHandler(const gpio_num_t GPIOnum,
                            const gpio_isr_t isrHandler) {
  gpio_isr_handler_add(GPIOnum, isrHandler, (void *)GPIOnum);
  ESP_LOGI(TAG_, "End of setting handler");
}

void GpioDriver::removeHandler(const gpio_num_t GPIOnum) {
  gpio_isr_handler_remove(GPIOnum);
}

bool GpioDriver::getLevel(const gpio_num_t GPIOnum) {
  return gpio_get_level(GPIOnum);
}

void GpioDriver::setLevel(const gpio_num_t GPIOnum, bool state) {
  gpio_set_level(GPIOnum, state);
}

} // namespace Gpio
} // namespace tamagotchi

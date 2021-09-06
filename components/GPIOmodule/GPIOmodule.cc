#include "GPIOmodule.hpp"

namespace tamagotchi {
const char* GPIOModule::TAG_ = "ESP32 GPIOModule";

void GPIOModule::GPIOinit(const gpio_num_t GPIOnum, const GPIOPullMode pull,
                          const GPIOIOmode mode, const GPIOedge edge) {
  gpio_config_t gpioConfig;
  switch (edge) {
    case GPIOedge::ANY:
      gpioConfig.intr_type = GPIO_INTR_ANYEDGE;
      break;
    case GPIOedge::RISING:
      gpioConfig.intr_type = GPIO_INTR_POSEDGE;
      break;
    case GPIOedge::FALLING:
      gpioConfig.intr_type = GPIO_INTR_NEGEDGE;
      break;
  }

  gpioConfig.mode =
      mode == GPIOIOmode::INPUT ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT;
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

void GPIOModule::GPIOinit(const std::vector<gpio_num_t>& GPIOnum,
                          const GPIOPullMode pull, const GPIOIOmode mode,
                          const GPIOedge edge) {
  for (const auto& num : GPIOnum) {
    GPIOinit(num, pull, mode, edge);
  }
  ESP_LOGI(TAG_, "End of GPIOInit");
}

void GPIOModule::setHandler(const std::vector<gpio_num_t>& GPIOnum,
                            const gpio_isr_t isrHandler) {
  for (const auto& num : GPIOnum) {
    setHandler(num, isrHandler);
  }
  ESP_LOGI(TAG_, "End of setting handlers");
}

void GPIOModule::setHandler(const gpio_num_t GPIOnum,
                            const gpio_isr_t isrHandler) {
  gpio_isr_handler_add(GPIOnum, isrHandler, (void*)GPIOnum);
  ESP_LOGI(TAG_, "End of setting handler");
}

void GPIOModule::removeHandler(const gpio_num_t GPIOnum) {
  gpio_isr_handler_remove(GPIOnum);
}

bool GPIOModule::getLevel(const gpio_num_t GPIOnum) {
  return gpio_get_level(GPIOnum);
}

void GPIOModule::setLevel(const gpio_num_t GPIOnum, bool state) {
  gpio_set_level(GPIOnum, state);
}

}  // namespace tamagotchi

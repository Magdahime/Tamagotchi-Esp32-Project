#include "GPIOmodule.hpp"
namespace tamagotchi {
namespace GPIOModule {
void GPIOinit(const gpio_num_t GPIOnum, const GPIOPullMode pull,
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
  ESP_LOGI(GPIOConsts::TAG, "GPIO[%d] configured", GPIOnum);
}

void GPIOinit(const std::vector<gpio_num_t>& GPIOnum, const GPIOPullMode pull,
              const GPIOIOmode mode, const GPIOedge edge) {
  for (const auto& num : GPIOnum) {
    GPIOinit(num, pull, mode, edge);
  }
}

void setHandler(const std::vector<gpio_num_t>& GPIOnum,
                const gpio_isr_t isrHandler) {
  for (const auto& num : GPIOnum) {
    setHandler(num, isrHandler);
  }
}

void setHandler(const gpio_num_t GPIOnum, const gpio_isr_t isrHandler) {
  gpio_isr_handler_add(GPIOnum, isrHandler, NULL);
}

void removeHandler(const gpio_num_t GPIOnum) {
  gpio_isr_handler_remove(GPIOnum);
}
}  // namespace GPIOModule

}  // namespace tamagotchi

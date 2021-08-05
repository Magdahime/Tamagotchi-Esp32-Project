#include "Joystick.hpp"

#include "GPIOconf.hpp"
#include "hal/gpio_types.h"

namespace tamagotchi {
xQueueHandle Joystick::joystickEventQueue = xQueueCreate(10, sizeof(uint32_t));

Joystick::Joystick(){}

void Joystick::init() {
  std::vector<gpio_num_t> gpios{GPIOInputs::GPIO_RST,   GPIOInputs::GPIO_SET,
                                GPIOInputs::GPIO_LEFT,  GPIOInputs::GPIO_RIGHT,
                                GPIOInputs::GPIO_UP,    GPIOInputs::GPIO_DOWN,
                                GPIOInputs::GPIO_MIDDLE};
  GPIOModule::GPIOinit(gpios, GPIOPullMode::PULLDOWN, GPIOIOmode::INPUT,
                       GPIOedge::RISING);
  gpio_install_isr_service(GPIOConsts::ESP_INTR_FLAG_DEFAULT);
  GPIOModule::setHandler(gpios, handler);
  ESP_LOGI(GPIOConsts::TAG, "Start task");
  xTaskCreate(task, "task", 2048, NULL, 10, NULL);
}

void Joystick::task(void *arg) {
  gpio_num_t io_num;
  for (;;) {
    if (xQueueReceive(joystickEventQueue, &io_num, portMAX_DELAY)) {
      ESP_LOGI("JOYSTICK MODULE", "Pressed %s",
               GPIOInputs::gpio2string.at(io_num));
    }
  }
}

void Joystick::handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(joystickEventQueue, &gpio_num, NULL);
}

}  // namespace tamagotchi

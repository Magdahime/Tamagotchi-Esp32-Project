#include "Joystick.hpp"

#include "GPIOconf.hpp"
#include "hal/gpio_types.h"

namespace tamagotchi {
xQueueHandle Joystick::joystickEventQueue = nullptr;
uint32_t Joystick::counter = 0;

Joystick::Joystick() {
  joystickEventQueue = xQueueCreate(10, sizeof(uint32_t));
}

void Joystick::init() {
  std::vector<gpio_num_t> gpios{GPIOInputs::GPIO_RST,   GPIOInputs::GPIO_SET,
                                GPIOInputs::GPIO_LEFT,  GPIOInputs::GPIO_RIGHT,
                                GPIOInputs::GPIO_UP,    GPIOInputs::GPIO_DOWN,
                                GPIOInputs::GPIO_MIDDLE};
  GPIOModule::GPIOinit(gpios, GPIOPullMode::PULLUP, GPIOIOmode::INPUT,
                       GPIOedge::RISING);
  xTaskCreate(task, "task", 2048, NULL, 10, NULL);

  gpio_install_isr_service(GPIOConsts::ESP_INTR_FLAG_DEFAULT);
  GPIOModule::setHandler(gpios, handler);

  while (1) {
    ESP_LOGI("JOYSTICK MODULE", "Counter is %d", counter);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void Joystick::task(void *arg) {
  gpio_num_t io_num;
  for (;;) {
    if (xQueueReceive(joystickEventQueue, &io_num, portMAX_DELAY)) {
      printf("Pressed button!");
    }
  }
}

void Joystick::handler(void *arg) {
  counter++;
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(joystickEventQueue, &gpio_num, NULL);
}

}  // namespace tamagotchi

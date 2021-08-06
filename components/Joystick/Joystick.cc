#include "Joystick.hpp"

#include <stdint.h>

#include "GPIOconf.hpp"
#include "GPIOmodule.hpp"
#include "hal/gpio_types.h"


namespace tamagotchi {

xQueueHandle Joystick::joystickEventQueue = xQueueCreate(10, sizeof(uint32_t));
volatile int Joystick::numberOfButtonInterrupts = 0;
volatile bool Joystick::lastState = false;
volatile uint32_t Joystick::debounceTimeout = 0;
volatile gpio_num_t Joystick::gpioNum;

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
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

  bool saveLastState;
  uint32_t saveDebounceTimeout;
  gpio_num_t saveGpioNum;
  bool save;

  for (;;) {
    taskENTER_CRITICAL(&mux);

    save = numberOfButtonInterrupts;
    saveDebounceTimeout = debounceTimeout;
    saveLastState = lastState;
    saveGpioNum = gpioNum;

    taskEXIT_CRITICAL(&mux);

    bool currentState =
        GPIOModule::getLevel(static_cast<gpio_num_t>(saveGpioNum));

    if (save != 0 && (currentState == saveLastState) &&
        (xTaskGetTickCount() - saveDebounceTimeout >
         GPIOConsts::DEBOUNCE_TIME) &&
        saveGpioNum == gpioNum) {
      ESP_LOGI("JOYSTICK MODULE", "Pressed %s",
               GPIOInputs::gpio2string.at(saveGpioNum));
      taskENTER_CRITICAL(&mux);
      numberOfButtonInterrupts = 0;
      gpioNum = static_cast<gpio_num_t>(0);
      taskEXIT_CRITICAL(&mux);
      vTaskDelay(GPIOConsts::SUCCESS_DELAY);
    }
    vTaskDelay(GPIOConsts::FAILURE_DELAY);
    // if (xQueueReceive(joystickEventQueue, &io_num, portMAX_DELAY)) {
    //   ESP_LOGI("JOYSTICK MODULE", "Pressed %s",
    //            GPIOInputs::gpio2string.at(io_num));
    // }
  }
}

void IRAM_ATTR Joystick::handler(void *arg) {
  // Enter critical section
  UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
  gpioNum = static_cast<gpio_num_t>((uint32_t)arg);
  numberOfButtonInterrupts++;
  lastState = GPIOModule::getLevel(static_cast<gpio_num_t>(gpioNum));
  debounceTimeout = xTaskGetTickCountFromISR();
  // xQueueSendFromISR(joystickEventQueue, &gpioNum, NULL);
  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

}  // namespace tamagotchi

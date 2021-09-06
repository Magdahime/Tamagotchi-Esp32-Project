#include "ESP_Joystick.hpp"

#include <stdint.h>

#include "GPIOconf.hpp"
#include "GPIOmodule.hpp"
#include "hal/gpio_types.h"


namespace tamagotchi {

xQueueHandle ESP_Joystick::joystickEventQueue = xQueueCreate(10, sizeof(uint32_t));
volatile int ESP_Joystick::numberOfButtonInterrupts_ = 0;
volatile bool ESP_Joystick::lastState_ = false;
volatile uint32_t ESP_Joystick::debounceTimeout_ = 0;
volatile gpio_num_t ESP_Joystick::gpioNum_;
 const char* ESP_Joystick::TAG_ = "ESP32 Joystick";

ESP_Joystick::ESP_Joystick(){
  GPIOModule::GPIOinit(GPIOInputs::gpios, GPIOPullMode::PULLDOWN, GPIOIOmode::INPUT,
                       GPIOedge::RISING);
  gpio_install_isr_service(GPIOConsts::ESP_INTR_FLAG_DEFAULT);
  GPIOModule::setHandler(GPIOInputs::gpios, handler);
  ESP_LOGI(TAG_, "Start task");
  xTaskCreate(task, "task", 2048, NULL, 10, NULL);
}

void ESP_Joystick::task(void *arg) {
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

  bool savelastState_;
  uint32_t savedebounceTimeout_;
  gpio_num_t savegpioNum_;
  bool save;
  
  for (;;) {
    taskENTER_CRITICAL(&mux);

    save = numberOfButtonInterrupts_;
    savedebounceTimeout_ = debounceTimeout_;
    savelastState_ = lastState_;
    savegpioNum_ = gpioNum_;

    taskEXIT_CRITICAL(&mux);

    bool currentState =
        GPIOModule::getLevel(static_cast<gpio_num_t>(savegpioNum_));

    if (save != 0 && (currentState == savelastState_) &&
        (xTaskGetTickCount() - savedebounceTimeout_ >
         GPIOConsts::DEBOUNCE_TIME) &&
        savegpioNum_ == gpioNum_) {
      ESP_LOGI("ESP_Joystick MODULE", "Pressed %s",
               GPIOInputs::gpio2string.at(savegpioNum_));
      taskENTER_CRITICAL(&mux);
      numberOfButtonInterrupts_ = 0;
      gpioNum_ = static_cast<gpio_num_t>(0);
      taskEXIT_CRITICAL(&mux);
      vTaskDelay(GPIOConsts::SUCCESS_DELAY);
    }
    vTaskDelay(GPIOConsts::FAILURE_DELAY);
  }
}

void IRAM_ATTR ESP_Joystick::handler(void *arg) {
  // Enter critical section
  UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
  gpioNum_ = static_cast<gpio_num_t>((uint32_t)arg);
  numberOfButtonInterrupts_++;
  lastState_ = GPIOModule::getLevel(static_cast<gpio_num_t>(gpioNum_));
  debounceTimeout_ = xTaskGetTickCountFromISR();
  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

}  // namespace tamagotchi

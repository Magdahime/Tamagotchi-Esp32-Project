#include "Joystick.hpp"

#include <stdint.h>

#include "Event.hpp"
#include "GPIOconf.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "GpioDriver.hpp"
#include "hal/gpio_types.h"

namespace tamagotchi {
namespace Joystick {
volatile int Joystick::numberOfButtonInterrupts_ = 0;
volatile bool Joystick::lastState_ = false;
volatile uint32_t Joystick::debounceTimeout_ = 0;
volatile gpio_num_t Joystick::gpioNum_;
const char *Joystick::TAG_ = "ESP32 Joystick";

void Joystick::task(void *arg) {
  Gpio::GpioDriver::GPIOinit(Gpio::GpioInputs::gpios,
                             Gpio::GPIOPullMode::PULLDOWN,
                             Gpio::GPIOIOmode::INPUT, Gpio::GPIOedge::RISING);
  gpio_install_isr_service(Gpio::consts::ESP_INTR_FLAG_DEFAULT);
  Gpio::GpioDriver::setHandler(Gpio::GpioInputs::gpios, handler);
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
        Gpio::GpioDriver::getLevel(static_cast<gpio_num_t>(savegpioNum_));

    if (save != 0 && (currentState == savelastState_) &&
        (xTaskGetTickCount() - savedebounceTimeout_ >
         Gpio::consts::DEBOUNCE_TIME) &&
        savegpioNum_ == gpioNum_) {
      taskENTER_CRITICAL(&mux);
      App::Event::Event event{App::Event::EventTypes::gpio, {}};
      *(reinterpret_cast<App::Event::GpioEvent *>(&event.data_)) =
          App::Event::GpioEvent{true, gpioNum_};
      App::Globals::game.putQueue(event);
      numberOfButtonInterrupts_ = 0;
      gpioNum_ = static_cast<gpio_num_t>(0);
      taskEXIT_CRITICAL(&mux);
      ESP_LOGI(TAG_, "Pressed %s",
               Gpio::GpioInputs::gpio2string.at(savegpioNum_));
      vTaskDelay(Gpio::consts::SUCCESS_DELAY);
    }
    vTaskDelay(Gpio::consts::FAILURE_DELAY);
  }
}

void IRAM_ATTR Joystick::handler(void *arg) {
  // Enter critical section
  UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
  gpioNum_ = static_cast<gpio_num_t>((uint32_t)arg);
  numberOfButtonInterrupts_++;
  lastState_ = Gpio::GpioDriver::getLevel(static_cast<gpio_num_t>(gpioNum_));
  debounceTimeout_ = xTaskGetTickCountFromISR();
  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}
}  // namespace Joystick
}  // namespace tamagotchi

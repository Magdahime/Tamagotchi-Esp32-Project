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
volatile gpio_num_t Joystick::gpioNum_;
const char *Joystick::TAG_ = "ESP32 Joystick";
SemaphoreHandle_t Joystick::mutex_ = xSemaphoreCreateBinary();

void Joystick::task(void *arg) {
  Gpio::GpioDriver::GPIOinit(Gpio::GpioInputs::gpios,
                             Gpio::GPIOPullMode::PULLDOWN,
                             Gpio::GPIOIOmode::INPUT, Gpio::GPIOedge::RISING);
  gpio_install_isr_service(Gpio::consts::ESP_INTR_FLAG_DEFAULT);
  Gpio::GpioDriver::setHandler(Gpio::GpioInputs::gpios, handler);
  bool savelastState, save;
  gpio_num_t savegpioNum;
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

  while (xSemaphoreTake(mutex_, 0xffff) == pdTRUE) {

    taskENTER_CRITICAL(&mux);

    save = numberOfButtonInterrupts_;
    savelastState = lastState_;
    savegpioNum = gpioNum_;

    taskEXIT_CRITICAL(&mux);
    bool currentState =
        Gpio::GpioDriver::getLevel(static_cast<gpio_num_t>(savegpioNum));

    if (save != 0 && (currentState == savelastState) &&
        savegpioNum == gpioNum_) {
      App::Event::Event event{App::Event::EventTypes::gpio};
      *(reinterpret_cast<App::Event::GpioEvent *>(&event.data_)) =
          App::Event::GpioEvent{true, gpioNum_};
      App::Globals::game.eventQueue().putQueue(event);
      numberOfButtonInterrupts_ = 0;
      gpioNum_ = static_cast<gpio_num_t>(0);
      ESP_LOGI(TAG_, "Pressed %s",
               Gpio::GpioInputs::gpio2string.at(savegpioNum));
    }
  }
}

void IRAM_ATTR Joystick::handler(void *arg) {
  UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
  gpioNum_ = static_cast<gpio_num_t>((uint32_t)arg);
  numberOfButtonInterrupts_++;
  lastState_ = Gpio::GpioDriver::getLevel(static_cast<gpio_num_t>(gpioNum_));
  xSemaphoreGiveFromISR(mutex_, NULL);
  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}
}  // namespace Joystick
}  // namespace tamagotchi

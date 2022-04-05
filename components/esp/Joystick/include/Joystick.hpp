#pragma once
#include <esp_log.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GpioDriver.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

namespace tamagotchi {
namespace Joystick {
class Joystick {
 public:
  int32_t checkButtonPress();
  static void task(void *arg);

 private:
  static void handler(void *arg);
  static SemaphoreHandle_t  mutex_;
  static volatile int numberOfButtonInterrupts_;
  static volatile bool lastState_;
  static volatile gpio_num_t gpioNum_;
  static const char *TAG_;
};
}  // namespace Joystick
}  // namespace tamagotchi

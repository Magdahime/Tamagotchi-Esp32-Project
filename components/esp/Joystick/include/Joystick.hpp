#pragma once
#include <esp_log.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GpioDriver.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

namespace tamagotchi {
class Joystick {
 public:
  Joystick();
  int32_t checkButtonPress();

 private:
  static void task(void *arg);
  static void handler(void *arg);
  static xQueueHandle joystickEventQueue;

  static volatile int numberOfButtonInterrupts_;
  static volatile bool lastState_;
  static volatile uint32_t debounceTimeout_;
  static volatile gpio_num_t gpioNum_;
  static const char* TAG_;
};


}  // namespace tamagotchi

#pragma once
#include <esp_log.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

#include "GPIOmodule.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"


namespace tamagotchi {
class Joystick {
 public:
  Joystick(){}
  void init();
  int32_t checkButtonPress();

 private:
  static void task(void *arg);
  static void handler(void *arg);
  static xQueueHandle joystickEventQueue;

  static volatile int numberOfButtonInterrupts;
  static volatile bool lastState;
  static volatile uint32_t debounceTimeout;
  static volatile gpio_num_t gpioNum;
};

}  // namespace tamagotchi

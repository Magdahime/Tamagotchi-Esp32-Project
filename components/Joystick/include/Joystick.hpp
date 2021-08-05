#pragma once
#include "GPIOmodule.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <esp_log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace tamagotchi {
class Joystick {
public:
  Joystick();
  void init();

private:
  static void task(void *arg);
  static void handler(void *arg);
  static xQueueHandle joystickEventQueue;
};

} // namespace tamagotchi

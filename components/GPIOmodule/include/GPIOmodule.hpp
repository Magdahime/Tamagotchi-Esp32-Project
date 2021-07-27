#ifndef GPIOMODULE
#define GPIOMODULE

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdio.h>
#include <esp_log.h>
#include <stdlib.h>
#include <string.h>
#include "GPIOconf.hpp"

namespace tamagotchi {
class GPIOModule {
  public:
  static void GPIOinit();
  static void GPIOtask(void*  arg);
  static void GPIOhandler(void* arg);
  private:
  static xQueueHandle gpioEventQueue;
  static uint32_t counter;
};

} // namespace tamagotchi

#endif
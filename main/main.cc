#include <cstdio>

#include "EspnowDriver.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Joystick.hpp"
#include "ST7789Driver.hpp"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace tamagotchi::App::Globals {
Game::Game game = tamagotchi::App::Game::Game();
}

// TASK FUNCTIONS
static void runGame(void* pvParameters);

// TASK HANDLERS
TaskHandle_t mainGameTaskHandle;
TaskHandle_t joystickTask;

extern "C" void app_main() {
  printf("Hello tamagotchi!\n");
  // esp_log_level_set("*", ESP_LOG_DEBUG);
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  xTaskCreate(runGame, "MainGameTask", 4096, NULL, tskIDLE_PRIORITY,
              &mainGameTaskHandle);
  xTaskCreate(tamagotchi::Joystick::Joystick::task, "JoystickTask", 4096, NULL,
              11, &joystickTask);
  vTaskStartScheduler();
}

static void runGame(void* pvParameters) {
  tamagotchi::App::Globals::game.run();
  vTaskDelete(mainGameTaskHandle);
  vTaskDelete(joystickTask);
}
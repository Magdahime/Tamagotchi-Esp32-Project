#include <assert.h>

#include <cstdio>

#include "Game.hpp"
#include "Globals.hpp"
#include "GomokuNetworking.hpp"
#include "Joystick.hpp"
#include "ST7789Driver.hpp"
#include "esp_crc.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_now.h"
#include "esp_random.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "nvs_flash.h"

#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF ESP_IF_WIFI_AP

static constexpr char TAG_[] = "Program main";
namespace tamagotchi::App::Globals {
Game::Game game = tamagotchi::App::Game::Game();

}

void initWifi();
void initEspNow();
static void runGame(void *pvParameters);

TaskHandle_t mainGameTaskHandle;
TaskHandle_t joystickTask;

// MAIN
extern "C" void app_main() {
  initWifi();
  initEspNow();
  ESP_LOGI(TAG_, "Creating tasks.");
  xTaskCreate(runGame, "MainGameTask", 4096, NULL, configMAX_PRIORITIES - 2,
              &mainGameTaskHandle);
  xTaskCreate(tamagotchi::Joystick::Joystick::task, "JoystickTask", 4096, NULL,
              configMAX_PRIORITIES - 1, &joystickTask);
}

// FUNCTIONS IMPLEMENTATION
static void runGame(void *pvParameters) {
  tamagotchi::App::Globals::game.run();
  vTaskDelete(mainGameTaskHandle);
  vTaskDelete(joystickTask);
}

/* Initialize Wifi to use EspNow */

void initWifi() {
  ESP_LOGI(TAG_, "Init Wifi started.");
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_LOGI(TAG_, "Init Wifi completed successfully.");
}

/* Initialize ESPNOW and register sending and receiving callback function. */
void initEspNow() {
  ESP_LOGI(TAG_, "Init ESPNOW started.");
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_send_cb(
      tamagotchi::App::GomokuNetworking::GomokuNetworking::sendDataCallback));
  ESP_ERROR_CHECK(
      esp_now_register_recv_cb(tamagotchi::App::GomokuNetworking::
                                   GomokuNetworking::receiveDataCallback));
  ESP_LOGI(TAG_, "Init ESPNOW completed successfully.");
}
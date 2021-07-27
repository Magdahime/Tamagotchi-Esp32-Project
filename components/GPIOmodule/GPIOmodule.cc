#include "GPIOmodule.hpp"
namespace tamagotchi {
xQueueHandle GPIOModule::gpioEventQueue = nullptr;
uint32_t GPIOModule::counter = 0;

void GPIOModule::GPIOinit() {

  gpio_config_t btn_config;
  btn_config.intr_type =
      GPIO_INTR_ANYEDGE; // Enable interrupt on both rising and falling edges
  btn_config.mode = GPIO_MODE_INPUT;                        // Set as Input
  btn_config.pin_bit_mask = (1ULL << GPIOInputs::GPIO_RST); // Bitmask
  btn_config.pull_up_en = GPIO_PULLUP_ENABLE;              // Disable pullup
  btn_config.pull_down_en = GPIO_PULLDOWN_DISABLE;           // Enable pulldown
  gpio_config(&btn_config);
  printf("Button configured\n");

  gpioEventQueue = xQueueCreate(10, sizeof(uint32_t));
  xTaskCreate(GPIOtask, "GPIOtask", 2048, NULL, 10, NULL);

  gpio_install_isr_service(GPIOConsts::ESP_INTR_FLAG_DEFAULT);
  gpio_isr_handler_add(GPIOInputs::GPIO_RST, GPIOhandler, NULL);
  printf("Interrupt configured\n");

  while (1) {
    ESP_LOGI("test", "Counter is %d", counter);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void GPIOModule::GPIOtask(void *arg) {
  gpio_num_t io_num;
  for (;;) {
    if (xQueueReceive(gpioEventQueue, &io_num, portMAX_DELAY)) {
      printf("Pressed button!");
    }
  }
}

void GPIOModule::GPIOhandler(void *arg) {
  counter++;
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(gpioEventQueue, &gpio_num, NULL);
}
} // namespace tamagotchi

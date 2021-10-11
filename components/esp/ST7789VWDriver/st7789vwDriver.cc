#include "st7789vwDriver.hpp"

namespace tamagotchi {
namespace ST7789 {

ST7789VWDriver::ST7789VWDriver(structs::st7789_config_t config)
    : spiDriver(consts::LCD_HOST) {
  gpioInit(config.gpio);
  lcdInit(config.lcd);
}

esp_err_t ST7789VWDriver::gpioInit(structs::gpio_config_t gpio) {
  spi_bus_config_t busconfig = {.mosi_io_num = gpio.gpioMosi,
                                .miso_io_num = -1,
                                .sclk_io_num = gpio.gpioSclk,
                                .quadwp_io_num = -1,
                                .quadhd_io_num = -1,
                                .max_transfer_sz = 0,
                                .flags = 0,
                                .intr_flags = 0};
  spiDriver.initialize(busconfig);

  spi_device_interface_config_t deviceConfig;
  memset(&deviceConfig, 0, sizeof(deviceConfig));
  deviceConfig.clock_speed_hz = consts::SPI_FREQUENCY;
  deviceConfig.queue_size = consts::QUEUE_SIZE;
  deviceConfig.mode = 2;
  deviceConfig.flags = SPI_DEVICE_NO_DUMMY;

  if (gpio.gpioCs >= 0) {
    deviceConfig.spics_io_num = gpio.gpioCs;
  } else {
    deviceConfig.spics_io_num = -1;
  }

  spiHandle_ = spiDriver.addDevice(&deviceConfig);
  dataCommand_ = gpio.gpioDc;
  backlight_ = gpio.gpioBl;
  return ESP_OK;
}

esp_err_t ST7789VWDriver::lcdInit(structs::lcd_config_t lcd) {
  width_ = lcd.width;
  height_ = lcd.height;
  offsetx_ = lcd.offsetx;
  offsety_ = lcd.offsety;
  return ESP_OK;
}

} // namespace ST7789
} // namespace tamagotchi
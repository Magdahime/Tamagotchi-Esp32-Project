#include "ST7789Driver.hpp"

#include <stdint.h>
#include <stdio.h>

#include <cmath>
#include <cstdint>
#include <cstdlib>

#include "SpiConf.hpp"

namespace tamagotchi {
namespace ST7789 {

const char *ST7789VWDriver::TAG_ = "ESP32 ST7789Driver";

inline void ST7789VWDriver::startCommand() {
  Gpio::GpioDriver::setLevel(static_cast<gpio_num_t>(dataCommand_),
                             consts::COMMAND);
}
inline void ST7789VWDriver::startDataTransfer() {
  Gpio::GpioDriver::setLevel(static_cast<gpio_num_t>(dataCommand_),
                             consts::DATA_TRANSFER);
}
void ST7789VWDriver::setDisplayAddress(uint16_t beginX, uint16_t endX,
                                       uint16_t beginY, uint16_t endY) {
  writeCommand(commands::columnAddressSet);
  writeAddress(beginX, endX);
  writeCommand(commands::rowAddressSet);
  writeAddress(beginY, endY);
}
ST7789VWDriver::ST7789VWDriver(structs::st7789_config_t config)
    : spiDriver_(consts::LCD_HOST) {
  gpioInit(config.gpio);
  lcdInit(config.lcd);
}

ST7789VWDriver::~ST7789VWDriver() { spiDriver_.removeDevice(spiHandle_); }

esp_err_t ST7789VWDriver::gpioInit(structs::gpio_config_t gpio) {
  ESP_LOGI(TAG_, "GPIO_CS=%d", gpio.gpioCs);
  if (gpio.gpioCs >= 0) {
    gpio_reset_pin(gpio.gpioCs);
    gpio_set_direction(gpio.gpioCs, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio.gpioCs, 0);
  }

  ESP_LOGI(TAG_, "GPIO_DC=%d", gpio.gpioDc);
  gpio_reset_pin(gpio.gpioDc);
  gpio_set_direction(gpio.gpioDc, GPIO_MODE_OUTPUT);
  gpio_set_level(gpio.gpioDc, 0);

  ESP_LOGI(TAG_, "GPIO_RESET=%d", gpio.gpioReset);
  if (gpio.gpioReset >= 0) {
    gpio_reset_pin(gpio.gpioReset);
    gpio_set_direction(gpio.gpioReset, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio.gpioReset, 1);
    tamagotchi::EspGL::delay(50);
    gpio_set_level(gpio.gpioReset, 0);
    tamagotchi::EspGL::delay(50);
    gpio_set_level(gpio.gpioReset, 1);
    tamagotchi::EspGL::delay(50);
  }

  ESP_LOGI(TAG_, "GPIO_BL=%d", gpio.gpioBl);
  if (gpio.gpioBl >= 0) {
    // gpio_pad_select_gpio(GPIO_BL);
    gpio_reset_pin(gpio.gpioBl);
    gpio_set_direction(gpio.gpioBl, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio.gpioBl, 0);
  }

  spi_bus_config_t busconfig = {.mosi_io_num = gpio.gpioMosi,
                                .miso_io_num = -1,
                                .sclk_io_num = gpio.gpioSclk,
                                .quadwp_io_num = -1,
                                .quadhd_io_num = -1,
                                .max_transfer_sz = 0,
                                .flags = 0,
                                .intr_flags = 0};
  spiDriver_.initialize(busconfig);

  spi_device_interface_config_t deviceConfig;
  memset(&deviceConfig, 0, sizeof(deviceConfig));
  deviceConfig.clock_speed_hz = consts::SPI_FREQUENCY;
  deviceConfig.queue_size = consts::QUEUE_SIZE;
  deviceConfig.mode = consts::MODE;
  deviceConfig.flags = SPI_DEVICE_NO_DUMMY;

  if (gpio.gpioCs >= 0) {
    deviceConfig.spics_io_num = gpio.gpioCs;
  } else {
    deviceConfig.spics_io_num = -1;
  }

  spiHandle_ = spiDriver_.addDevice(&deviceConfig);
  dataCommand_ = gpio.gpioDc;
  backlight_ = gpio.gpioBl;
  return ESP_OK;
}

esp_err_t ST7789VWDriver::lcdInit(structs::lcd_config_t lcd) {
  width_ = lcd.width;
  height_ = lcd.height;
  offsetx_ = lcd.offsetx;
  offsety_ = lcd.offsety;

  writeCommand(commands::softwareReset);
  tamagotchi::EspGL::delay(consts::LONG_PAUSE);

  writeCommand(commands::sleepOut);
  tamagotchi::EspGL::delay(consts::MEDIUM_PAUSE);

  writeCommand(commands::interfacePixelFormat);
  writeByte(consts::PIXEL_FORMAT);
  tamagotchi::EspGL::delay(consts::SHORT_PAUSE);

  writeCommand(commands::memoryDataAccessControl);
  writeByte(consts::MEMORY_ACCESS_FORMAT);

  writeCommand(commands::displayInversionOn);
  tamagotchi::EspGL::delay(consts::SHORT_PAUSE);

  writeCommand(commands::partialModeOff);
  tamagotchi::EspGL::delay(consts::SHORT_PAUSE);

  writeCommand(commands::displayOn);
  tamagotchi::EspGL::delay(consts::LONG_PAUSE);

  ESP_LOGI(TAG_, "Successfully initialized ST7789Driver");

  return ESP_OK;
}

void ST7789VWDriver::writeAddress(uint16_t address1, uint16_t address2) {
  address1 = (address1 << 8) + (address1 >> 8);
  address2 = (address2 << 8) + (address2 >> 8);
  struct {
    uint16_t a1, a2;
  } data{address1, address2};
  writeBytes(reinterpret_cast<uint8_t *>(&data), sizeof data);
}

void ST7789VWDriver::writeCommand(uint8_t command) {
  startCommand();
  ESP_LOGD(TAG_, "Writing command: 0x%X", command);
  spiDriver_.writeCommand(spiHandle_, command);
}

void ST7789VWDriver::writeBytes(const uint8_t *bytes, size_t size) {
  startDataTransfer();
  spiDriver_.writeBytes(spiHandle_, bytes, size);
}
void ST7789VWDriver::writeByte(const uint8_t byte) { writeBytes(&byte, 1); }

void ST7789VWDriver::writeColour(uint16_t colour) {
  colour = (colour << 8) + (colour >> 8);
  startDataTransfer();
  spiDriver_.writeDataWords(spiHandle_, &colour, 1);
}

void ST7789VWDriver::writeColour(uint16_t colour, size_t size) {
  colour = (colour << 8) + (colour >> 8);
  startDataTransfer();
  for (auto counter = 0; counter < size; counter++) {
    spiDriver_.writeBytes(spiHandle_, reinterpret_cast<const uint8_t *>(&colour),
                          tamagotchi::Spi::consts::DATA_WORD_BYTES);
  }
}

void ST7789VWDriver::drawPixel(const EspGL::Point &point, uint16_t colour) {
  if (point.x_ >= width_ || point.y_ >= height_) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixel()! Pixel "
             "coordinates are out of bound!");
    return;
  }
  uint16_t finalX = point.x_ + offsetx_;
  uint16_t finalY = point.y_ + offsety_;
  ESP_LOGD(TAG_, "Drawing pixel at: %d:%d in colour: 0x%X", point.x_, point.y_,
           colour);
  setDisplayAddress(finalX, finalX, finalY, finalY);
  writeCommand(commands::memoryWrite);
  writeColour(colour);
}

void ST7789VWDriver::drawPixelLine(const EspGL::Point &point, uint16_t size,
                                   uint16_t colour) {
  if (point.x_ + size > width_ || point.y_ >= height_) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixelLine()! Pixel "
             "coordinates are out of bound!");
    return;
  }

  uint16_t beginX = point.x_ + offsetx_;
  uint16_t endX = beginX + size;
  uint16_t beginY = point.y_ + offsety_;
  ESP_LOGD(TAG_, "Drawing line of pixels: %d:%d  in %d line in colour: 0x%X",
           beginX, endX, beginY, colour);
  setDisplayAddress(beginX, endX, beginY, beginY);
  writeCommand(commands::memoryWrite);
  writeColour(colour, size);
}

void ST7789VWDriver::writePixelArea(int16_t startX, int16_t endX,
                                    int16_t startY, int16_t endY,
                                    uint16_t colour) {
  setDisplayAddress(startX, endX, startY, endY);
  writeCommand(commands::memoryWrite);
  writeColour(colour, std::abs(endX - startX) * std::abs(endY - startY));
}

void ST7789VWDriver::writePixelArea(int16_t startX, int16_t endX,
                                    int16_t startY, int16_t endY,
                                    std::vector<uint16_t> &data) {
  setDisplayAddress(startX, endX, startY, endY);
  writeCommand(commands::memoryWrite);
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(data.data());
  writeBytes(bytes, data.size() * tamagotchi::Spi::consts::DATA_WORD_BYTES);
}

std::vector<uint16_t> ST7789VWDriver::readDisplayMemory(
    const EspGL::Point &start, const EspGL::Point &stop) {
  writeCommand(commands::memoryRead);
  setDisplayAddress(start.x_, stop.x_, start.y_, stop.y_);
  size_t size = abs((stop.x_ - start.x_) * (stop.y_ - start.y_));
  std::vector<uint16_t> vectorData(size);
  spiDriver_.readDataWords(spiHandle_, vectorData.data(),
                           size * tamagotchi::Spi::consts::DATA_WORD_BYTES);
  return vectorData;
}

}  // namespace ST7789
}  // namespace tamagotchi
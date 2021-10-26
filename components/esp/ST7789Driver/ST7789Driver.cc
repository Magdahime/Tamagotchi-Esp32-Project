#include "ST7789Driver.hpp"

namespace tamagotchi {
namespace ST7789 {

const char *ST7789VWDriver::TAG_ = "ESP32 ST7789Driver";

void ST7789VWDriver::delay(uint32_t ms) {
  TickType_t delay = ms / portTICK_PERIOD_MS;
  ESP_LOGD(TAG_, "Delay for: %dms, %d ticks", ms, delay);
  vTaskDelay(delay);
}

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
    delay(50);
    gpio_set_level(gpio.gpioReset, 0);
    delay(50);
    gpio_set_level(gpio.gpioReset, 1);
    delay(50);
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
  delay(consts::LONG_PAUSE);

  writeCommand(commands::sleepOut);
  delay(consts::MEDIUM_PAUSE);

  writeCommand(commands::interfacePixelFormat);
  writeByte(consts::PIXEL_FORMAT);
  delay(consts::SHORT_PAUSE);

  writeCommand(commands::memoryDataAccessControl);
  writeByte(consts::MEMORY_ACCESS_FORMAT);

  writeCommand(commands::columnAddressSet);
  writeByte(0x00);
  writeByte(0x00);
  writeByte(0x00);
  writeByte(0xF0);

  writeCommand(commands::rowAddressSet);
  writeByte(0x00);
  writeByte(0x00);
  writeByte(0x00);
  writeByte(0xF0);

  writeCommand(commands::displayInversionOn);
  delay(consts::SHORT_PAUSE);

  writeCommand(commands::partialModeOff);
  delay(consts::SHORT_PAUSE);

  writeCommand(commands::displayOn);
  delay(consts::LONG_PAUSE);

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
  ESP_LOGI(TAG_, "Writing command: 0x%X", command);
  spiDriver_.writeCommand(spiHandle_, command);
}

void ST7789VWDriver::writeBytes(uint8_t *bytes, size_t size) {
  startDataTransfer();
  spiDriver_.writeBytes(spiHandle_, bytes, size);
}
void ST7789VWDriver::writeByte(uint8_t byte) { writeBytes(&byte, 1); }

void ST7789VWDriver::writeColour(uint16_t colour) {
  colour = (colour << 8) + (colour >> 8);
  startDataTransfer();
  spiDriver_.writeDataWords(spiHandle_, &colour, 1);
}

void ST7789VWDriver::writeColour(uint16_t colour, size_t size) {
  colour = (colour << 8) + (colour >> 8);
  uint16_t data[size];
  for (auto counter = 0; counter < size; counter++) {
    data[counter] = colour;
  }
  startDataTransfer();
  spiDriver_.writeDataWords(spiHandle_, data, size);
}

void ST7789VWDriver::drawPixel(uint16_t x, uint16_t y, uint16_t colour) {
  if (x >= width_ || y >= height_) {
    ESP_LOGE(TAG_, "Incorrect data were provided to drawPixel()! Pixel "
                   "coordinates are out of bound!");
    return;
  }
  uint16_t finalX = x + offsetx_;
  uint16_t finalY = y + offsety_;
  ESP_LOGI(TAG_, "Drawing pixel at: %d:%d in colour: 0x%X", x, y, colour);
  setDisplayAddress(finalX, finalX, finalY, finalY);
  writeCommand(commands::memoryWrite);
  writeColour(colour);
}

void ST7789VWDriver::drawPixelLine(uint16_t x, uint16_t y, uint16_t size,
                                   uint16_t colour) {
  if (x + size > width_ || y >= height_) {
    ESP_LOGE(TAG_, "Incorrect data were provided to drawPixelLine()! Pixel "
                   "coordinates are out of bound!");
    return;
  }

  uint16_t beginX = x + offsetx_;
  uint16_t endX = beginX + size;
  uint16_t beginY = y + offsety_;
  ESP_LOGI(TAG_, "Drawing line of pixels: %d:%d  in %d line in colour: 0x%X",
           beginX, endX, beginY, colour);
  setDisplayAddress(beginX, endX, beginY, beginY);
  writeCommand(commands::memoryWrite);
  writeColour(colour);
}

void ST7789VWDriver::drawFilledRectangle(uint16_t x1, uint16_t y1, uint16_t x2,
                                         uint16_t y2, uint16_t colour) {
  if (x1 >= width_ || y1 >= height_) {
    ESP_LOGE(TAG_, "Incorrect data were provided to drawPixel()! Rectangle "
                   "vertices are out of bound!");
    return;
  }
  if (x2 >= width_)
    x2 = width_ - 1;
  if (y2 >= height_)
    y2 = height_ - 1;

  uint16_t beginX = x1 + offsetx_;
  uint16_t endX = x2 + offsetx_;
  uint16_t beginY = y1 + offsety_;
  uint16_t endY = y2 + offsety_;
  uint16_t size = endX - beginX + 1;
  ESP_LOGI(TAG_,
           "Drawing rectangle of vertices: (%d,%d) (%d,%d) (%d,%d) (%d,%d) in "
           "colour: 0x%X",
           beginX, beginY, endX, beginY, endX, endY, beginX, endY, colour);
  ESP_LOGI(TAG_, "Length of the side: %d ", size);
  setDisplayAddress(beginX, endX, beginY, endY);
  writeCommand(commands::memoryWrite);
  for (auto y = beginY; y <= endY; y++) {
    writeColour(colour, size);
  }
}
void ST7789VWDriver::fillScreen(uint16_t colour) {
  drawFilledRectangle(0, 0, width_ - 1, height_ - 1, colour);
}
} // namespace ST7789
} // namespace tamagotchi
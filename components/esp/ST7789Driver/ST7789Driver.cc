#include "ST7789Driver.hpp"

#include "ST7789Utils.hpp"

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
  ESP_LOGD(TAG_, "Writing command: 0x%X", command);
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

void ST7789VWDriver::drawPixel(const Point &point, uint16_t colour) {
  if (point.x >= width_ || point.y >= height_) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixel()! Pixel "
             "coordinates are out of bound!");
    return;
  }
  uint16_t finalX = point.x + offsetx_;
  uint16_t finalY = point.y + offsety_;
  ESP_LOGD(TAG_, "Drawing pixel at: %d:%d in colour: 0x%X", point.x, point.y,
           colour);
  setDisplayAddress(finalX, finalX, finalY, finalY);
  writeCommand(commands::memoryWrite);
  writeColour(colour);
}

void ST7789VWDriver::drawPixelLine(const Point &point, uint16_t size,
                                   uint16_t colour) {
  if (point.x + size > width_ || point.y >= height_) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixelLine()! Pixel "
             "coordinates are out of bound!");
    return;
  }

  uint16_t beginX = point.x + offsetx_;
  uint16_t endX = beginX + size;
  uint16_t beginY = point.y + offsety_;
  ESP_LOGD(TAG_, "Drawing line of pixels: %d:%d  in %d line in colour: 0x%X",
           beginX, endX, beginY, colour);
  setDisplayAddress(beginX, endX, beginY, beginY);
  writeCommand(commands::memoryWrite);
  writeColour(colour);
}

void ST7789VWDriver::drawFilledRectangle(const Point &point1,
                                         const Point &point2, uint16_t colour) {
  if (point1.x >= width_ || point1.y >= height_) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixel()! Rectangle "
             "vertices are out of bound!");
    return;
  }
  uint16_t beginX = point1.x + offsetx_;
  uint16_t endX = point2.x + offsetx_;
  uint16_t beginY = point1.y + offsety_;
  uint16_t endY = point2.y + offsety_;
  uint16_t size = endX - beginX + 1;
  if (point2.x >= width_) endX = width_ - 1 + offsetx_;
  if (point2.y >= height_) endY = height_ - 1 + offsety_;
  ESP_LOGD(TAG_,
           "Drawing filled rectangle of vertices: (%d,%d) (%d,%d) (%d,%d) "
           "(%d,%d) in "
           "colour: 0x%X",
           beginX, beginY, endX, beginY, endX, endY, beginX, endY, colour);
  ESP_LOGD(TAG_, "Length of the side: %d ", size);
  setDisplayAddress(beginX, endX, beginY, endY);
  writeCommand(commands::memoryWrite);
  for (auto y = beginY; y <= endY; y++) {
    writeColour(colour, size);
  }
}
void ST7789VWDriver::fillScreen(uint16_t colour) {
  drawFilledRectangle(
      {0, 0},
      {static_cast<int16_t>(width_ - 1), static_cast<int16_t>(height_ - 1)},
      colour);
}

// Implementation of Bresenham Algorithm
void ST7789VWDriver::drawLine(const Point &point1, const Point &point2,
                              uint16_t colour) {
  ESP_LOGD(TAG_,
           "Drawing line: (%d,%d) -> (%d,%d) in "
           "colour: 0x%X",
           point1.x, point1.y, point2.x, point2.y, colour);
  int16_t kx = point2.x >= point1.x ? 1 : -1;
  int16_t ky = point2.y >= point1.y ? 1 : -1;
  int16_t dx = abs(point2.x - point1.x);
  int16_t dy = abs(point2.y - point1.y);
  drawPixel(point1, colour);

  int16_t currX = point1.x;
  int16_t currY = point1.y;
  auto bresenhamLoop = [&](int16_t &v1, int16_t &v2, int16_t &k1, int16_t &k2,
                           int16_t &d1, int16_t &d2) {
    double err = d1 / 2.0;
    for (auto i = 0; i < d1; ++i) {
      v1 = v1 + k1;
      err = err - d2;
      if (err < 0) {
        v2 = v2 + k2;
        err = err + d1;
      }
      drawPixel({currX, currY}, colour);
    }
  };
  if (dx >= dy) {
    bresenhamLoop(currX, currY, kx, ky, dx, dy);
  } else {
    bresenhamLoop(currY, currX, ky, kx, dy, dx);
  }
}

void ST7789VWDriver::drawRectangle(const Point &point1, const Point &point2,
                                   uint16_t colour) {
  ESP_LOGD(TAG_,
           "Drawing rectangle of vertices: (%d,%d) (%d,%d) (%d,%d) (%d,%d) in "
           "colour: 0x%X",
           point1.x, point1.y, point2.x, point1.y, point2.x, point2.y, point1.x,
           point2.y, colour);
  drawLine({point1.x, point1.y}, {point2.x, point1.y}, colour);
  drawLine({point2.x, point1.y}, {point2.x, point2.y}, colour);
  drawLine({point2.x, point2.y}, {point1.x, point2.y}, colour);
  drawLine({point1.x, point2.y}, {point1.x, point1.y}, colour);
}

void ST7789VWDriver::drawTriangle(const Point &point1, const Point &point2,
                                  const Point &point3, uint16_t colour) {
  ESP_LOGD(TAG_,
           "Drawing triangle of vertices: (%d,%d) (%d,%d) (%d,%d) in "
           "colour: 0x%X",
           point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, colour);
  drawLine(point1, point2, colour);
  drawLine(point2, point3, colour);
  drawLine(point3, point1, colour);
}

void ST7789VWDriver::drawFilledTriangle(const Point &point1,
                                        const Point &point2,
                                        const Point &point3, uint16_t colour) {
  auto [max, mid, min] = sort3Points(point1, point2, point3, Coordinate::Y);
  for (auto y = min.y; y <= mid.y; ++y) {
    auto point1 = straightLineEquation(min, mid, {{}, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {{}, y}, Coordinate::X);
    drawLine(point1, point2, colour);
  }
  for (auto y = mid.y; y <= max.y; ++y) {
    auto point1 = straightLineEquation(mid, max, {{}, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {{}, y}, Coordinate::X);
    drawLine(point1, point2, colour);
  }
}

}  // namespace ST7789
}  // namespace tamagotchi
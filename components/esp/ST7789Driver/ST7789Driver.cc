#include "ST7789Driver.hpp"

#include <stdint.h>

#include <cmath>
#include <cstdint>

#include "ST7789Conf.hpp"
#include "ST7789Utils.hpp"
#include "SpiConf.hpp"

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
  uint16_t data[size];
  for (auto counter = 0; counter < size; counter++) {
    data[counter] = colour;
  }
  startDataTransfer();
  spiDriver_.writeDataWords(spiHandle_, data, size);
}

void ST7789VWDriver::drawPixel(const Point &point, uint16_t colour) {
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

void ST7789VWDriver::drawPixelLine(const Point &point, uint16_t size,
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

void ST7789VWDriver::drawFilledRectangle(const Point &point1,
                                         const Point &point2, uint16_t colour) {
  Point min_point = {std::min(point1.x_, point2.x_),
                     std::min(point1.y_, point2.y_)};
  Point max_point = {std::max(point1.x_, point2.x_),
                     std::max(point1.y_, point2.y_)};
  if (min_point.x_ >= width_ || min_point.y_ >= height_) {
    ESP_LOGE(TAG_,
             "Incorrect data was provided to drawPixel()! Rectangle "
             "vertices are out of bound!");
    return;
  }

  uint16_t beginX = min_point.x_ + offsetx_;
  uint16_t endX = max_point.x_ + offsetx_;
  uint16_t beginY = min_point.y_ + offsety_;
  uint16_t endY = max_point.y_ + offsety_;
  uint16_t size = endX - beginX + 1;
  if (max_point.x_ >= width_) endX = width_ - 1 + offsetx_;
  if (max_point.y_ >= height_) endY = height_ - 1 + offsety_;
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
  drawFilledRectangle({0, 0}, {width_ - 1, height_ - 1}, colour);
}

// Implementation of Bresenham Algorithm
void ST7789VWDriver::drawLine(const Point &point1, const Point &point2,
                              uint16_t colour) {
  ESP_LOGD(TAG_,
           "Drawing line: (%d,%d) -> (%d,%d) in "
           "colour: 0x%X",
           point1.x_, point1.y_, point2.x_, point2.y_, colour);
  int16_t kx = point2.x_ >= point1.x_ ? 1 : -1;
  int16_t ky = point2.y_ >= point1.y_ ? 1 : -1;
  int16_t dx = abs(point2.x_ - point1.x_);
  int16_t dy = abs(point2.y_ - point1.y_);
  drawPixel(point1, colour);

  int16_t currX = point1.x_;
  int16_t currY = point1.y_;
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
           point1.x_, point1.y_, point2.x_, point1.y_, point2.x_, point2.y_,
           point1.x_, point2.y_, colour);
  drawLine({point1.x_, point1.y_}, {point2.x_, point1.y_}, colour);
  drawLine({point2.x_, point1.y_}, {point2.x_, point2.y_}, colour);
  drawLine({point2.x_, point2.y_}, {point1.x_, point2.y_}, colour);
  drawLine({point1.x_, point2.y_}, {point1.x_, point1.y_}, colour);
}

void ST7789VWDriver::drawTriangle(const Point &point1, const Point &point2,
                                  const Point &point3, uint16_t colour) {
  ESP_LOGD(TAG_,
           "Drawing triangle of vertices: (%d,%d) (%d,%d) (%d,%d) in "
           "colour: 0x%X",
           point1.x_, point1.y_, point2.x_, point2.y_, point3.x_, point3.y_,
           colour);
  drawLine(point1, point2, colour);
  drawLine(point2, point3, colour);
  drawLine(point3, point1, colour);
}

void ST7789VWDriver::drawFilledTriangle(const Point &point1,
                                        const Point &point2,
                                        const Point &point3, uint16_t colour) {
  auto [max, mid, min] = sort3Points(point1, point2, point3, Coordinate::Y);
  for (auto y = min.y_; y <= mid.y_; ++y) {
    auto point1 = straightLineEquation(min, mid, {{}, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {{}, y}, Coordinate::X);
    drawLine(point1, point2, colour);
  }
  for (auto y = mid.y_; y <= max.y_; ++y) {
    auto point1 = straightLineEquation(mid, max, {{}, y}, Coordinate::X);
    auto point2 = straightLineEquation(min, max, {{}, y}, Coordinate::X);
    drawLine(point1, point2, colour);
  }
}

// Implementation of Mid-Point algorithm
void ST7789VWDriver::drawCircle(const Point &center, double r,
                                uint16_t colour) {
  ESP_LOGD(TAG_,
           "Drawing circle of center (%d,%d) and radius %f in "
           "colour: 0x%X",
           center.x_, center.y_, r, colour);
  int16_t x = r;
  int16_t y = 0;
  int16_t err = 0;
  while (x >= y) {
    drawPixel({center.x_ + x, center.y_ + y}, colour);
    drawPixel({center.x_ + y, center.y_ + x}, colour);
    drawPixel({center.x_ - y, center.y_ + x}, colour);
    drawPixel({center.x_ - x, center.y_ + y}, colour);
    drawPixel({center.x_ - x, center.y_ - y}, colour);
    drawPixel({center.x_ - y, center.y_ - x}, colour);
    drawPixel({center.x_ + y, center.y_ - x}, colour);
    drawPixel({center.x_ + x, center.y_ - y}, colour);

    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }

    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}
void ST7789VWDriver::drawFilledCircle(const Point &center, double r,
                                      uint16_t colour) {
  ESP_LOGD(TAG_,
           "Drawing filled circle of center (%d,%d) and radius %f in "
           "colour: 0x%X",
           center.x_, center.y_, r, colour);
  int16_t x = r;
  int16_t y = 0;
  int16_t err = 0;
  while (x >= y) {
    drawLine({center.x_ + x, center.y_ + y}, {center.x_ - x, center.y_ + y},
             colour);
    drawLine({center.x_ + y, center.y_ + x}, {center.x_ - y, center.y_ + x},
             colour);
    drawLine({center.x_ - x, center.y_ - y}, {center.x_ + x, center.y_ - y},
             colour);
    drawLine({center.x_ - y, center.y_ - x}, {center.x_ + y, center.y_ - x},
             colour);

    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }

    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

void ST7789VWDriver::drawPolygon(const Point &center, double r,
                                 uint16_t vertices, uint16_t colour,
                                 double rotation) {
  ESP_LOGD(TAG_,
           "Drawing polygon of center (%d,%d) and radius %f and %d vertices in "
           "colour: 0x%X",
           center.x_, center.y_, r, vertices, colour);
  double gamma = 2.0 * consts::PI / vertices;
  Point lastPoint{center.x_ + sin(rotation) * r, center.y_ + cos(rotation) * r};
  for (auto i = 1; i <= vertices; ++i) {
    Point currentPoint{center.x_ + sin(rotation + i * gamma) * r,
                       center.y_ + cos(rotation + i * gamma) * r};
    drawLine(lastPoint, currentPoint, colour);
    std::swap(lastPoint, currentPoint);
  }
}
void ST7789VWDriver::drawFilledPolygon(const Point &center, double r,
                                       uint16_t vertices, uint16_t colour,
                                       double rotation) {
  ESP_LOGD(TAG_,
           "Drawing filled polygon of center (%d,%d) and radius %f and %d "
           "vertices in "
           "colour: 0x%X",
           center.x_, center.y_, r, vertices, colour);
  double gamma = 2.0 * consts::PI / vertices;
  Point lastPoint{center.x_ + sin(rotation) * r, center.y_ + cos(rotation) * r};
  for (auto i = 1; i <= vertices; ++i) {
    Point currentPoint{center.x_ + sin(rotation + i * gamma) * r,
                       center.y_ + cos(rotation + i * gamma) * r};
    ESP_LOGI(TAG_, "(%d, %d) (%d, %d) (%d, %d)", lastPoint.x_, lastPoint.y_,
             currentPoint.x_, currentPoint.y_, center.x_, center.y_);
    drawFilledTriangle(lastPoint, currentPoint, center, colour / i);
    std::swap(lastPoint, currentPoint);
  }
}

std::vector<uint16_t> ST7789VWDriver::readDisplayMemory(const Point &start,
                                                        const Point &stop) {
  writeCommand(commands::memoryRead);
  setDisplayAddress(start.x_, stop.x_, start.y_, stop.y_);
  size_t size = abs((stop.x_ - start.x_) * (stop.y_ - start.y_));
  std::vector<uint16_t> vectorData(size);
  spiDriver_.readDataWords(spiHandle_, vectorData.data(),
                           size * tamagotchi::Spi::consts::DATA_WORD_BYTES);
  return vectorData;
}

void ST7789VWDriver::drawBitmap(const Point &start, uint16_t sizeX,
                                uint16_t sizeY, uint16_t colour,
                                const std::vector<bool> &bitmap) {
  if (sizeY * sizeX == bitmap.size()) {
    ESP_LOGD(TAG_, "Drawing bitmap of size (%d, %d)in colour: 0x%X", sizeX,
             sizeY, colour);
    uint16_t countX = 0, countY = 0;
    for (const auto &pixel : bitmap) {
      if (pixel) {
        drawPixel({start.x_ + countX, start.y_ + countY}, colour);
      }
      if (++countX == sizeX) {
        countY++;
        countX = 0;
      }
    }
  } else {
    ESP_LOGE(TAG_, "Incorrect data given to drawBitmap()! Incorrect size.");
  }
}
void ST7789VWDriver::drawPicture(const Point &start, uint16_t sizeX,
                                 uint16_t sizeY,
                                 const std::vector<uint16_t> &picture) {
  setDisplayAddress(start.x_, start.x_ + sizeX, start.y_, start.y_ + sizeY);
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(picture.data());
  writeBytes(bytes, picture.size() * tamagotchi::Spi::consts::DATA_WORD_BYTES);
}
}  // namespace ST7789
}  // namespace tamagotchi
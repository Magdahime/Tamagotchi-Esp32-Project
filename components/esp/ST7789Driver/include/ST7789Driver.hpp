#pragma once
#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <utility>

#include "GpioDriver.hpp"
#include "ST7789Conf.hpp"
#include "ST7789Utils.hpp"
#include "SpiDriver.hpp"

namespace tamagotchi {
namespace ST7789 {

class ST7789VWDriver {
 public:
  ST7789VWDriver(structs::st7789_config_t config);
  ~ST7789VWDriver();
  void delay(uint32_t ms);
  void drawPixel(const Point &point, uint16_t colour);
  void drawPixelLine(const Point &point, uint16_t size, uint16_t colour);
  void drawFilledRectangle(const Point &point1, const Point &point2,
                           uint16_t colour);
  void fillScreen(uint16_t colour);
  void drawLine(const Point &point1, const Point &point2, uint16_t colour);
  void drawRectangle(const Point &point1, const Point &point2, uint16_t colour);
  void drawTriangle(const Point &point1, const Point &point2,
                    const Point &point3, uint16_t colour);
  void drawFilledTriangle(const Point &point1, const Point &point2,
                          const Point &point3, uint16_t colour);
  void drawCircle(const Point &center, double r, uint16_t colour);
  void drawFilledCircle(const Point &center, double r, uint16_t colour);
  void drawPolygon(const Point &center, double r, uint16_t vertices,
                   uint16_t colour, double rotation = 0.0);
  void drawFilledPolygon(const Point &center, double r, uint16_t vertices,
                         uint16_t colour, double rotation = 0.0);
  void drawBitmap(const Point &start, uint16_t sizeX, uint16_t sizeY,
                  uint16_t colour, const std::vector<bool> &bitmap);
  void drawEllipse(const Point &center, double xRadius, double yRadius,
                   uint16_t colour);
  void drawFilledEllipse(const Point &center, double xRadius, double yRadius,
                   uint16_t colour);
  void drawPicture(const Point &start, uint16_t sizeX, uint16_t sizeY,
                   const std::vector<uint16_t> &picture);
  uint16_t convertColour(uint8_t r, uint8_t g, uint8_t b);

 private:
  static const char *TAG_;

  inline void startCommand();
  inline void startDataTransfer();
  esp_err_t gpioInit(structs::gpio_config_t gpio);
  esp_err_t lcdInit(structs::lcd_config_t lcd);
  void writeCommand(uint8_t command);
  void writeBytes(const uint8_t *bytes, size_t size);
  void writeByte(const uint8_t byte);
  void writeAddress(uint16_t address1, uint16_t address2);
  void setDisplayAddress(uint16_t beginX, uint16_t endX, uint16_t beginY,
                         uint16_t endY);
  void writeColour(uint16_t colour);
  void writeColour(uint16_t colour, size_t size);
  std::vector<uint16_t> readDisplayMemory(const Point &start,
                                          const Point &stop);

  Spi::SpiDriver spiDriver_;
  uint16_t width_;
  uint16_t height_;
  uint16_t offsetx_;
  uint16_t offsety_;
  uint16_t dataCommand_;
  uint16_t backlight_;
  uint64_t spiHandle_;
};

// TODO
// class Operation {
// public:
// private:
// };

// class DataTransfer : public Operation {
// public:
// private:
// }

// class Command : public Operation {
// public:
// private:
// }

}  // namespace ST7789
}  // namespace tamagotchi
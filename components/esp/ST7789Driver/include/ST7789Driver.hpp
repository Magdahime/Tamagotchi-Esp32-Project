#pragma once
#include "GpioDriver.hpp"
#include "ST7789Conf.hpp"
#include "SpiDriver.hpp"

namespace tamagotchi {
namespace ST7789 {
class ST7789VWDriver {
public:
  ST7789VWDriver(structs::st7789_config_t config);
  ~ST7789VWDriver();
  void delay(uint32_t ms);
  void drawPixel(uint16_t x, uint16_t y, uint16_t colour);
  void drawPixelLine(uint16_t x, uint16_t y, uint16_t size, uint16_t colour);
  void drawFilledRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                           uint16_t colour);
  void fillScreen(uint16_t colour);
  void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                uint16_t colour);
  void drawRectangle(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,
                     uint16_t colour);
  void drawRectAngle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h,
                     uint16_t angle, uint16_t colour);
  void drawTriangle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h,
                    uint16_t angle, uint16_t colour);
  void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t colour);
  void drawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t colour);
  void drawRoundRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                          uint16_t r, uint16_t colour);
  void drawArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w,
                 uint16_t colour);
  void drawFilledArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                       uint16_t w, uint16_t colour);

private:
  static const char *TAG_;
  inline void startCommand();
  inline void startDataTransfer();
  esp_err_t gpioInit(structs::gpio_config_t gpio);
  esp_err_t lcdInit(structs::lcd_config_t lcd);
  void writeCommand(uint8_t command);
  void writeBytes(uint8_t *bytes, size_t size);
  void writeByte(uint8_t byte);
  void writeAddress(uint16_t address1, uint16_t address2);
  void setDisplayAddress(uint16_t beginX, uint16_t endX, uint16_t beginY,
                         uint16_t endY);
  void writeColour(uint16_t colour);
  void writeColour(uint16_t colour, size_t size);
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

} // namespace ST7789
} // namespace tamagotchi
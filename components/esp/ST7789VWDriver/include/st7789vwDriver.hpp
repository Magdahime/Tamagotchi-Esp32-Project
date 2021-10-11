#include "GpioDriver.hpp"
#include "SpiDriver.hpp"
#include "st7789vwConf.hpp"

namespace tamagotchi {
namespace ST7789 {
class ST7789VWDriver {
public:
  ST7789VWDriver(structs::st7789_config_t config);
  void drawPixel(uint16_t x, uint16_t y, uint16_t color);
  void drawMultiPixels(uint16_t x, uint16_t y, uint16_t size, uint16_t *colors);
  void drawFilledRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                      uint16_t color);
  void fillScreen(uint16_t color);
  void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                uint16_t color);
  void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                     uint16_t color);
  void drawRectAngle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h,
                     uint16_t angle, uint16_t color);
  void drawTriangle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h,
                    uint16_t angle, uint16_t color);
  void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  void drawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  void drawRoundRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                          uint16_t r, uint16_t color);
  void drawArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w,
                 uint16_t color);
  void drawFilledArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                       uint16_t w, uint16_t color);

private:
  esp_err_t gpioInit(structs::gpio_config_t gpio);
  esp_err_t lcdInit(structs::lcd_config_t lcd);
  Spi::SpiDriver spiDriver;
  uint16_t width_;
  uint16_t height_;
  uint16_t offsetx_;
  uint16_t offsety_;
  uint16_t dataCommand_;
  uint16_t backlight_;
  uint64_t spiHandle_;
};
} // namespace ST7789
} // namespace tamagotchi
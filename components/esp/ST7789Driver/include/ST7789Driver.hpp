#pragma once
#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <utility>

#include "EspGLDriver.hpp"
#include "EspGLUtils.hpp"
#include "GpioDriver.hpp"
#include "ST7789Conf.hpp"
#include "SpiDriver.hpp"

namespace tamagotchi {
namespace ST7789 {

class ST7789VWDriver : public tamagotchi::EspGL::Driver<uint16_t> {
 public:
  ST7789VWDriver(structs::st7789_config_t config);
  ~ST7789VWDriver();
  virtual void drawPixel(const EspGL::Vect2 &point, uint16_t colour) override;

  virtual void writePixelArea(int16_t startX, int16_t endX, int16_t startY,
                              int16_t endY, uint16_t colour) override;

  virtual void writePixelArea(int16_t startX, int16_t endX, int16_t startY,
                              int16_t endY,
                              std::vector<uint16_t> &data) override;

  inline std::array<uint16_t, Spi::consts::SPI_BUFFER_SIZE> &dataBuffer() {
    return dataBuffer_;
  }

 private:
  static const char *TAG_;

  inline void startCommand();
  inline void startDataTransfer();
  esp_err_t gpioInit(structs::gpio_config_t gpio);
  esp_err_t lcdInit(structs::lcd_config_t lcd);
  virtual void writeCommand(uint8_t command) override;
  virtual void writeBytes(const uint8_t *bytes, size_t size) override;
  virtual void writeByte(const uint8_t byte) override;

  void writeAddress(uint16_t address1, uint16_t address2);
  void setDisplayAddress(uint16_t beginX, uint16_t endX, uint16_t beginY,
                         uint16_t endY);
  virtual void writeColour(uint16_t colour) override;
  virtual void writeColour(uint16_t colour, size_t size) override;
  std::vector<uint16_t> readDisplayMemory(const EspGL::Vect2 &start,
                                          const EspGL::Vect2 &stop);

  Spi::SpiDriver spiDriver_;
  std::array<uint16_t, Spi::consts::SPI_BUFFER_SIZE> &dataBuffer_;
  uint16_t width_;
  uint16_t height_;
  uint16_t offsetx_;
  uint16_t offsety_;
  uint16_t dataCommand_;
  uint16_t backlight_;
  uint64_t spiHandle_;
};

}  // namespace ST7789
}  // namespace tamagotchi
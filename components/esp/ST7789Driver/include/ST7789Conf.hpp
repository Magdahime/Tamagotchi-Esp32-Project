#pragma once
#include <stdint.h>

#include "driver/spi_common.h"
#include "driver/spi_master.h"

namespace tamagotchi {
namespace ST7789 {

namespace colours {
constexpr uint16_t RED = 0xf800;
constexpr uint16_t GREEN = 0x07e0;
constexpr uint16_t BLUE = 0x001f;
constexpr uint16_t BLACK = 0x0000;
constexpr uint16_t WHITE = 0xffff;
constexpr uint16_t GRAY = 0x8c51;
constexpr uint16_t YELLOW = 0xFFE0;
constexpr uint16_t CYAN = 0x07FF;
constexpr uint16_t PURPLE = 0xF81F;
}  // namespace colours

namespace consts {
constexpr int GPIOMOSI = 13;
constexpr int GPIOSCLK = 14;
constexpr int GPIOCS = 15;
constexpr int GPIODC = 2;
constexpr int GPIORESET = 4;
constexpr int GPIOBL = -1;
constexpr int WIDTH = 240;
constexpr int HEIGHT = 320;
constexpr int OFFSETX = 0;
constexpr int OFFSETY = 0;
}  // namespace consts

namespace structs {

struct gpio_config_t {
  gpio_num_t gpioMosi;
  gpio_num_t gpioSclk;
  gpio_num_t gpioCs;
  gpio_num_t gpioDc;
  gpio_num_t gpioReset;
  gpio_num_t gpioBl;
};

struct lcd_config_t {
  uint16_t width;
  uint16_t height;
  uint16_t offsetx;
  uint16_t offsety;
};

struct st7789_config_t {
  gpio_config_t gpio;
  lcd_config_t lcd;
};

}  // namespace structs

namespace consts {
constexpr double PI = 3.141592653589793238463;
constexpr spi_host_device_t LCD_HOST = HSPI_HOST;
constexpr int QUEUE_SIZE = 7;
constexpr int SPI_FREQUENCY = SPI_MASTER_FREQ_20M;
constexpr int MODE = 2;
constexpr bool DATA_TRANSFER = 1;
constexpr bool COMMAND = 0;
constexpr uint8_t PIXEL_FORMAT = 0b01010101;
constexpr uint8_t MEMORY_ACCESS_FORMAT = 0;
constexpr uint32_t SHORT_PAUSE = 1000;
constexpr uint32_t MEDIUM_PAUSE = 2000;
constexpr uint32_t LONG_PAUSE = 3000;
}  // namespace consts

namespace commands {
constexpr uint8_t noOperation = 0x00;
constexpr uint8_t softwareReset = 0x01;
constexpr uint8_t readDisplayId = 0x04;
constexpr uint8_t readDisplayStatus = 0x09;
constexpr uint8_t readDisplayPower = 0x0A;
constexpr uint8_t readDisplay = 0x0B;
constexpr uint8_t readDisplayPixel = 0x0C;
constexpr uint8_t readDisplayImage = 0x0D;
constexpr uint8_t readDisplaySignal = 0x0E;
constexpr uint8_t readDisplaySelfDiagnose = 0x0F;
constexpr uint8_t sleepIn = 0x10;
constexpr uint8_t sleepOut = 0x11;
constexpr uint8_t partialModeOn = 0x12;
constexpr uint8_t partialModeOff = 0x13;
constexpr uint8_t displayInversionOn = 0x21;
constexpr uint8_t displayInversionOff = 0x26;
constexpr uint8_t displayOff = 0x28;
constexpr uint8_t displayOn = 0x29;
constexpr uint8_t columnAddressSet = 0x2A;
constexpr uint8_t rowAddressSet = 0x2B;
constexpr uint8_t memoryWrite = 0x2C;
constexpr uint8_t memoryRead = 0x2E;
constexpr uint8_t partialStartEndAddressSet = 0x30;
constexpr uint8_t verticalScrollingDefinition = 0x33;
constexpr uint8_t tearingEffectLineOff = 0x34;
constexpr uint8_t tearingEffectLineOn = 0x35;
constexpr uint8_t memoryDataAccessControl = 0x36;
constexpr uint8_t verticalScrollingStartAddress = 0x37;
constexpr uint8_t idleModeOff = 0x38;
constexpr uint8_t idleModeOn = 0x39;
constexpr uint8_t interfacePixelFormat = 0x3A;
constexpr uint8_t memoryWriteContinue = 0x3C;
constexpr uint8_t memoryReadContinue = 0x3E;
constexpr uint8_t setTearScanline = 0x3E;
constexpr uint8_t getScanline = 0x45;
constexpr uint8_t writeDisplayBrightnes = 0x51;
constexpr uint8_t readDisplayBrightness = 0x52;
constexpr uint8_t writeCTRLDisplayValue = 0x53;
constexpr uint8_t readCTRLDisplayValue = 0x54;
constexpr uint8_t writeCABC = 0x55;
constexpr uint8_t readCABC = 0x56;
constexpr uint8_t writeCABCMinBrightness = 0x5E;
constexpr uint8_t readCABCMinBrightness = 0x5F;
constexpr uint8_t readABCSelfDiagnosticResult = 0x68;
constexpr uint8_t readID1 = 0xDA;
constexpr uint8_t readID2 = 0xDB;
constexpr uint8_t readID3 = 0xDC;
}  // namespace commands

}  // namespace ST7789
}  // namespace tamagotchi
#pragma once
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include <stdint.h>

namespace tamagotchi {
namespace ST7789 {

namespace structs {

struct gpio_config_t {
  int8_t gpioMosi;
  int8_t gpioSclk;
  int8_t gpioCs;
  int8_t gpioDc;
  int8_t gpioReset;
  int8_t gpioBl;
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

} // namespace structs

namespace consts {
constexpr spi_host_device_t LCD_HOST = HSPI_HOST;
constexpr int QUEUE_SIZE = 7;
constexpr int SPI_FREQUENCY = SPI_MASTER_FREQ_20M;
constexpr int MODE = 2;
} // namespace consts

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
} // namespace commands

} // namespace ST7789
} // namespace tamagotchi
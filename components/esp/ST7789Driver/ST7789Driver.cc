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

ST7789VWDriver::ST7789VWDriver(structs::st7789_config_t config)
    : spiDriver_(consts::LCD_HOST) {
  gpioInit(config.gpio);
  lcdInit(config.lcd);
}

ST7789VWDriver::~ST7789VWDriver() { spiDriver_.removeDevice(spiHandle_); }

esp_err_t ST7789VWDriver::gpioInit(structs::gpio_config_t gpio) {
  ESP_LOGI(TAG_, "GPIO_CS=%d", gpio.gpioCs);
  if (gpio.gpioCs >= 0) {
    // gpio_pad_select_gpio( GPIO_CS );
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
    // gpio_pad_select_gpio( GPIO_RESET );
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
  struct {
    uint16_t a1, a2;
  } data{address1, address2};
  writeBytes(reinterpret_cast<uint8_t *>(&data), sizeof data);
}

void ST7789VWDriver::writeCommand(uint8_t command) {
  startCommand();
  spiDriver_.writeCommand(spiHandle_, command);
}

void ST7789VWDriver::writeBytes(uint8_t *bytes, size_t size) {
  startDataTransfer();
  spiDriver_.writeBytes(spiHandle_, bytes, size);
}
void ST7789VWDriver::writeByte(uint8_t byte) { writeBytes(&byte, 1); }

void ST7789VWDriver::writeColour(uint16_t colour) {
  startDataTransfer();
  spiDriver_.writeDataWords(spiHandle_, &colour, 1);
}

void ST7789VWDriver::drawPixel(uint16_t x, uint16_t y, uint16_t colour) {
  if (x >= width_ || y >= height_)
    return;

  uint16_t finalX = x + offsetx_;
  uint16_t finalY = y + offsety_;
  writeCommand(commands::columnAddressSet);
  writeAddress(finalX, finalX);
  writeCommand(commands::rowAddressSet);
  writeAddress(finalY, finalY);
  writeCommand(commands::memoryWrite);
  writeColour(colour);
}



} // namespace ST7789
} // namespace tamagotchi
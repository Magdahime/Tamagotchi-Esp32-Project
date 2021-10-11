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

namespace commands{

}




} // namespace ST7789
} // namespace tamagotchi
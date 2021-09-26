#include "SpiDriver.hpp"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"

namespace tamagotchi {
uint8_t SpiDriver::deviceCounter = 0;
const char* SpiDriver::TAG_ = "ESP32 SpiDriver";

esp_err_t SpiDriver::initialize(int mosiNum, int misoNum, int sclkNum,
                                int quadwpNum, int quadhdNum, int maxTransfer) {
  spi_bus_config_t config;
  config.mosi_io_num = mosiNum;
  config.miso_io_num = misoNum;
  config.sclk_io_num = sclkNum;
  config.quadwp_io_num = quadwpNum;
  config.quadhd_io_num = quadhdNum;
  config.max_transfer_sz = maxTransfer;
  return spi_bus_initialize(host_, &config, 0);
}



}  // namespace tamagotchi

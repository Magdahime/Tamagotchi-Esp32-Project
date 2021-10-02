#include "SpiDriver.hpp"

namespace tamagotchi {
namespace Spi {
const char *SpiDriver::TAG_ = "ESP32 SpiDriver";

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

int8_t SpiDriver::addDevice(const spi_device_interface_config_t *dev_config) {
  spi_device_handle_t handle;
  if (spi_bus_add_device(host_, dev_config, &handle) != ESP_FAIL && !devices.full()) {
    auto result = devices.insert(handle);
    return result;
  }
  return ESP_FAIL;
}

} // namespace Spi
} // namespace tamagotchi

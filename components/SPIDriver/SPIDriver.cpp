#include "SPIDriver.hpp"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"


namespace tamagotchi {
const char* SPIDriver::TAG_ = "ESP32 SPIDriver";

esp_err_t SPIDriver::initialize(int mosiNum, int misoNum, int sclkNum,
                           int quadwpNum, int quadhdNum,
                           int maxTransfer) {
  spi_bus_config_t config;
  config.mosi_io_num = mosiNum;
  config.miso_io_num = misoNum;
  config.sclk_io_num = sclkNum;
  config.quadwp_io_num = quadwpNum;
  config.quadhd_io_num = quadhdNum;
  config.max_transfer_sz = maxTransfer;
  return spi_bus_initialize(host_, &config, 0);
}

esp_err_t SPIDriver::close() { return spi_bus_free(host_); }

esp_err_t SPIDriver::addDevice(const spi_host_device_t& host,
                               const spi_bus_config_t& bus_config,
                               const spi_device_interface_config_t& dev_config,
                               const spi_device_handle_t& handle) {
  return spi_bus_free(host_);
}
esp_err_t SPIDriver::removeDevice(spi_device_handle_t handle) {
  return spi_bus_remove_device(handle);
}
}  // namespace tamagotchi

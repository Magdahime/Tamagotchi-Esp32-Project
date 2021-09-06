#pragma once

#include <esp_log.h>

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"

namespace tamagotchi {

class SPIDriver {
 public:
  SPIDriver(spi_host_device_t host) : host_(host){};
  ~SPIDriver() { close(); };

  esp_err_t initialize(int mosiNum, int misoNum, int sclkNum,
                       int quadwpNum = -1, int quadhdNum = -1,
                       int maxTransfer = SPI_MAX_DMA_LEN);
  esp_err_t close();

  esp_err_t addDevice(const spi_host_device_t &host,
                      const spi_bus_config_t &bus_config,
                      const spi_device_interface_config_t &dev_config,
                      const spi_device_handle_t &handle);

  esp_err_t addDevice(spi_device_interface_config_t *dev_config,
                      spi_device_handle_t *handle);
  esp_err_t removeDevice(spi_device_handle_t handle);

  esp_err_t writeBit(spi_device_handle_t handle, uint8_t regAddr,
                     uint8_t bitNum, uint8_t data);
  esp_err_t writeBits(spi_device_handle_t handle, uint8_t regAddr,
                      uint8_t bitStart, uint8_t length, uint8_t data);
  esp_err_t writeByte(spi_device_handle_t handle, uint8_t regAddr,
                      uint8_t data);
  esp_err_t writeBytes(spi_device_handle_t handle, uint8_t regAddr,
                       size_t length, const uint8_t *data);

  esp_err_t readBit(spi_device_handle_t handle, uint8_t regAddr, uint8_t bitNum,
                    uint8_t *data);
  esp_err_t readBits(spi_device_handle_t handle, uint8_t regAddr,
                     uint8_t bitStart, uint8_t length, uint8_t *data);
  esp_err_t readByte(spi_device_handle_t handle, uint8_t regAddr,
                     uint8_t *data);
  esp_err_t readBytes(spi_device_handle_t handle, uint8_t regAddr,
                      size_t length, uint8_t *data);

 private:
  spi_host_device_t host_;
  static const char *TAG_;
};

}  // namespace tamagotchi

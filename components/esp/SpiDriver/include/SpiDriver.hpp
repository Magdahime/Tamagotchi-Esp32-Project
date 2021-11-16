#pragma once
#include <stdint.h>

#include <cstring>

#include "RecyclingContainer.hpp"
#include "SpiConf.hpp"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/semphr.h"

namespace tamagotchi {

namespace Spi {
class SpiDriver {
 public:
  SpiDriver(spi_host_device_t host) : host_(host){};
  ~SpiDriver() { spi_bus_free(host_); };

  esp_err_t initialize(const int mosiNum, const int misoNum, const int sclkNum,
                       const int quadwpNum = -1, const int quadhdNum = -1,
                       const int maxTransfer = SPI_MAX_DMA_LEN);
  esp_err_t initialize(const spi_bus_config_t &config);
  size_t addDevice(const spi_device_interface_config_t *dev_config);
  esp_err_t removeDevice(const uint64_t descriptor);
  esp_err_t transaction(const uint64_t descriptor,
                        spi_transaction_t *transaction);
  esp_err_t writeBytes(const uint64_t descriptor, const uint8_t *data,
                       const size_t dataLength);
  esp_err_t writeByte(const uint64_t descriptor, const uint8_t data);
  esp_err_t writeDataWords(const uint64_t descriptor, const uint16_t *data,
                           const size_t dataLength);
  esp_err_t writeCommand(const uint64_t descriptor, const uint8_t command);
  esp_err_t readBytes(const uint64_t descriptor, uint8_t *data,
                      const size_t dataLength);
  esp_err_t readByte(const uint64_t descriptor, uint8_t *data);
  esp_err_t readDataWords(const uint64_t descriptor, uint16_t *data,
                          const size_t dataLength);

 private:
  spi_host_device_t host_;
  RecyclingContainer<spi_device_handle_t, consts::MAX_NUMBER_SPI_DEVICES>
      devices_;
  static const char *TAG_;
};
}  // namespace Spi
}  // namespace tamagotchi

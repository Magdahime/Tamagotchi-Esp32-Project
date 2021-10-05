#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "RecyclingContainer.hpp"
#include "SpiConf.hpp"

namespace tamagotchi {

namespace Spi {
class SpiDriver {
public:
  SpiDriver(spi_host_device_t host)
      : host_(host), transactionSem_(xSemaphoreCreateMutex()){};
  ~SpiDriver() { spi_bus_free(host_); };

  esp_err_t initialize(int mosiNum, int misoNum, int sclkNum,
                       int quadwpNum = -1, int quadhdNum = -1,
                       int maxTransfer = SPI_MAX_DMA_LEN);
  size_t addDevice(const spi_device_interface_config_t *dev_config);
  esp_err_t removeDevice(uint64_t descriptor);
  esp_err_t transatcion(uint64_t descriptor, spi_transaction_t* transaction);

private:
  spi_host_device_t host_;
  SemaphoreHandle_t transactionSem_;
  RecyclingContainer<spi_device_handle_t, consts::MAX_NUMBER_SPI_DEVICES>
      devices_;
  static const char *TAG_;
};
} // namespace Spi
} // namespace tamagotchi

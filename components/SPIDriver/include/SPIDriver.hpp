#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/semphr.h"


#include "RecyclingContainer.hpp"

namespace tamagotchi {

class SPIDriver {
 public:
  SPIDriver(spi_host_device_t host)
      : host_(host),
        transactionSem_(xSemaphoreCreateMutex()){};
  ~SPIDriver() { spi_bus_free(host_); };

  esp_err_t initialize(int mosiNum, int misoNum, int sclkNum,
                       int quadwpNum = -1, int quadhdNum = -1,
                       int maxTransfer = SPI_MAX_DMA_LEN);
  spi_device_t addDevice(const spi_device_interface_config_t* dev_config,
                         spi_device_handle_t* handle);
  esp_err_t removeDevice(spi_device_t handle);
  esp_err_t selectDevice(spi_device_t handle);
  esp_err_t deselectDevice(spi_device_t handle);
  esp_err_t transferData(spi_device_t handle,
                         const spi_transaction_t& transaction);
  esp_err_t sync_transferData(spi_device_t handle,
                              const spi_transaction_t& transaction);

 private:
  static uint8_t deviceCounter;
  spi_host_device_t host_;
  SemaphoreHandle_t transactionSem_;
  static const char* TAG_;
};

}  // namespace tamagotchi

#include "SpiDriver.hpp"

namespace tamagotchi {
namespace Spi {
const char *SpiDriver::TAG_ = "ESP32 SpiDriver";

esp_err_t SpiDriver::initialize(const int mosiNum, const int misoNum,
                                const int sclkNum, const int quadwpNum,
                                const int quadhdNum, const int maxTransfer) {
  spi_bus_config_t config;
  config.mosi_io_num = mosiNum;
  config.miso_io_num = misoNum;
  config.sclk_io_num = sclkNum;
  config.quadwp_io_num = quadwpNum;
  config.quadhd_io_num = quadhdNum;
  config.max_transfer_sz = maxTransfer;
  esp_err_t err = spi_bus_initialize(host_, &config, SPI_DMA_CH_AUTO);
  if (err == ESP_OK)
    ESP_LOGI(TAG_, "Successfully initialized SPI bus");
  else
    ESP_LOGE(TAG_, "FAIL: Cannot initialize SPI bus");
  return err;
}

esp_err_t SpiDriver::initialize(const spi_bus_config_t &config) {
  esp_err_t err = spi_bus_initialize(host_, &config, SPI_DMA_CH_AUTO);
  if (err == ESP_OK)
    ESP_LOGI(TAG_, "Successfully initialized SPI bus");
  else
    ESP_LOGE(TAG_, "FAIL: Cannot initialize SPI bus");
  return err;
}

size_t SpiDriver::addDevice(const spi_device_interface_config_t *dev_config) {
  spi_device_handle_t handle;
  if (spi_bus_add_device(host_, dev_config, &handle) != ESP_FAIL &&
      !devices_.full()) {
    auto result = devices_.insert(handle);
    ESP_LOGI(TAG_, "Adding new SPI device at: %lld", result);
    return result;
  }
  ESP_LOGE(TAG_, "FAIL: Cannot add new SPI device");
  return ESP_FAIL;
}

esp_err_t SpiDriver::removeDevice(const uint64_t descriptor) {
  esp_err_t err = spi_bus_remove_device(devices_[descriptor]);
  if (err == ESP_OK)
    ESP_LOGI(TAG_, "Removing SPI device at: %llu", descriptor);
  else
    ESP_LOGE(TAG_, "FAIL: Cannot remove SPI device at: %llu", descriptor);
  return err;
}

esp_err_t SpiDriver::transaction(const uint64_t descriptor,
                                 spi_transaction_t *transaction) {
  esp_err_t err = spi_device_transmit(devices_[descriptor], transaction);
  if (err == ESP_OK)
    ESP_LOGI(TAG_,
             "Command %d: Successfully received %d bits transmitted %d bits",
             transaction->cmd, transaction->rxlength, transaction->length);
  else
    ESP_LOGE(TAG_, "FAIL! Command:%d", transaction->cmd);
  return err;
}

esp_err_t SpiDriver::writeBytes(const uint64_t descriptor, const uint8_t *data,
                                const size_t dataLength) {
  if (dataLength > 0 && devices_[descriptor]) {
    spi_transaction_t spiTransaction;
    memset(&spiTransaction, 0, sizeof(spi_transaction_t));
    spiTransaction.length = dataLength * consts::BYTE;
    spiTransaction.tx_buffer = data;
    esp_err_t err = spi_device_transmit(devices_[descriptor], &spiTransaction);
    if (err == ESP_OK)
      ESP_LOGI(TAG_, "Successfully transmitted %d bits", dataLength);
    else
      ESP_LOGE(TAG_, "FAIL! Cannot transmit %d bits", dataLength);
  }
  ESP_LOGE(TAG_, "FAIL! Incorrect data provided to writeBytes()");
  return ESP_FAIL;
}

esp_err_t SpiDriver::writeDataWords(const uint64_t descriptor, uint16_t *data,
                                    const size_t dataLength) {
  if (dataLength > 0 && devices_[descriptor]) {
    uint8_t *bytes = reinterpret_cast<uint8_t *>(data);
    writeBytes(descriptor, bytes, dataLength * consts::DATA_WORD_BYTES);
  }
  ESP_LOGE(TAG_, "FAIL! Incorrect data provided to writeBytes()");
  return ESP_FAIL;
}

esp_err_t SpiDriver::writeCommand(const uint64_t descriptor,
                                  const uint8_t command) {
  return writeBytes(descriptor, &command, 1);
}

} // namespace Spi
} // namespace tamagotchi
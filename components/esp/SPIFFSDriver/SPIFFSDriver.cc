#include "SPIFFSDriver.hpp"

#include <fstream>

namespace tamagotchi {
namespace SPIFFS {

std::fstream SPIFFSDriver::getFileDescriptor(std::string filename) {
  std::string fullPath = basePath_ + "/" + filename;
  std::fstream fileHandle(fullPath);
  if (!fileHandle.is_open()) {
    ESP_LOGE(TAG_, "Failed to open file %s", filename.c_str());
  }
  return fileHandle;
}

SPIFFSDriver::SPIFFSDriver(std::string basePath, std::string partitionLabel,
                           uint16_t maxFiles, bool formatIfMountFailed)
    : basePath_(basePath),
      partitionLabel_(partitionLabel),
      maxFiles_(maxFiles),
      formatIfMountFailed_(formatIfMountFailed) {
  ESP_LOGI(TAG_, "Initializing SPIFFS");

  esp_vfs_spiffs_conf_t conf = {.base_path = basePath_.c_str(),
                                .partition_label = partitionLabel_.c_str(),
                                .max_files = maxFiles_,
                                .format_if_mount_failed = formatIfMountFailed_};

  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(TAG_, "Failed to mount or format filesystem");
    } else if (ret == ESP_ERR_NOT_FOUND) {
      ESP_LOGE(TAG_, "Failed to find SPIFFS partition");
    } else {
      ESP_LOGE(TAG_, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
    return;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(conf.partition_label, &total, &used);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG_, "Failed to get SPIFFS partition information (%s)",
             esp_err_to_name(ret));
  } else {
    ESP_LOGI(TAG_, "Partition size: total: %d, used: %d", total, used);
  }
}

SPIFFSDriver::~SPIFFSDriver() {
  esp_vfs_spiffs_unregister(partitionLabel_.c_str());
  ESP_LOGI(TAG_, "SPIFFS unmounted");
}

}  // namespace SPIFFS
}  // namespace tamagotchi

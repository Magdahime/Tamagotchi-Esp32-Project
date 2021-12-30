#pragma once

#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include <string>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

namespace tamagotchi {
namespace SPIFFS {
static const char* TAG_ = "SPIFFSDriver";

class SPIFFSDriver {
 public:
  SPIFFSDriver(std::string basePath = "/spiffs",
               std::string partitionLabel = "storage", uint16_t maxFiles = 5,
               bool formatIfMountFailed = false);
  ~SPIFFSDriver();
  FILE* getFileDescriptor(std::string filename, std::string mode);

 private:
  std::string basePath_;
  std::string partitionLabel_;
  uint16_t maxFiles_;
  bool formatIfMountFailed_;
};
}  // namespace SPIFFS
}  // namespace tamagotchi
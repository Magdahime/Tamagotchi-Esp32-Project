#pragma once

#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include <fstream>
#include <string>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

namespace tamagotchi {
namespace SPIFFS {
constexpr auto TAG_ = "SPIFFSDriver";

class SPIFFSDriver {
 public:
  SPIFFSDriver(std::string basePath = "/spiffs",
               std::string partitionLabel = "storage", uint16_t maxFiles = 5,
               bool formatIfMountFailed = false);
  ~SPIFFSDriver();
  std::fstream getFileDescriptor(std::string filename);

 private:
  std::string basePath_;
  std::string partitionLabel_;
  uint16_t maxFiles_;
  bool formatIfMountFailed_;
};


}  // namespace SPIFFS
}  // namespace tamagotchi
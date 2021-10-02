#pragma once

#include <stdint.h>
namespace tamagotchi {

namespace Spi {

namespace structs {
typedef struct {
  int mosiNum;
  int misoNum;
  int sclkNum;
  int quadwpNum;
  int quadhdNum;
  int maxTransfer;
} spi_bus_config_t;

} // namespace structs

namespace consts{
constexpr int MAX_NUMBER_SPI_DEVICES = 10;
}

} // namespace Spi

} // namespace tamagotchi
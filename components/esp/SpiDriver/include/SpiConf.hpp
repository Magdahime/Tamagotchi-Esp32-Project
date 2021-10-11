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

namespace consts {
constexpr int MAX_NUMBER_SPI_DEVICES = 10;
constexpr char BYTE = 8;
constexpr char DATA_WORD_BYTES = 2;
} // namespace consts

} // namespace Spi

} // namespace tamagotchi
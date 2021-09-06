#pragma once

#include <stdint.h>
namespace tamagotchi {

namespace structs {


typedef struct {
    int mosiNum;
    int misoNum;
    int sclkNum;
    int quadwpNum;
    int quadhdNum;
    int maxTransfer;
} spi_bus_config_t;



}  // namespace structs

}  // namespace tamagotchi
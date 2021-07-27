#ifndef GPIOCONF
#define GPIOCONF

namespace tamagotchi {

namespace GPIOInputs {
constexpr gpio_num_t GPIO_RST = GPIO_NUM_4;

}

namespace GPIOConsts {
constexpr int ESP_INTR_FLAG_DEFAULT = 0;
}
} // namespace tamagotchi

#endif
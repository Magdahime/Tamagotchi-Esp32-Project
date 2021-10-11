idf_component_register(SRCS "main.cc"
                    REQUIRES GpioDriver EspnowDriver Joystick spi_flash SpiDriver ST7789VWDriver)
component_compile_options(-std=gnu++17)
idf_component_register(SRCS "main.cc"
                    REQUIRES GpioDriver EspnowDriver Joystick spi_flash SpiDriver ST7789Driver)
component_compile_options(-std=gnu++17)
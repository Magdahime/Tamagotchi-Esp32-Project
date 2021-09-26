idf_component_register(SRCS "main.cc"
                    REQUIRES GpioDriver EspnowDriver Joystick spi_flash SpiDriver)
component_compile_options(-std=gnu++17)
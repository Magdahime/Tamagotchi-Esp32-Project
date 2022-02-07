idf_component_register(SRCS "ST7789Driver.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES GpioDriver SpiDriver EspGL )
component_compile_options(-std=gnu++17)
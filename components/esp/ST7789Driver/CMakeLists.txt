idf_component_register(SRCS "ST7789Driver.cc" "ST7789Utils.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES GpioDriver SpiDriver)
component_compile_options(-std=gnu++17)
idf_component_register(SRCS "main.cc"
                    REQUIRES GPIOmodule ESPNOWmodule ESP_Joystick spi_flash SPIDriver)
component_compile_options(-std=gnu++17)
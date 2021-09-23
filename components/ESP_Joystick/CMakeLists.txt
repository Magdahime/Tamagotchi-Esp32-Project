idf_component_register(SRCS "ESP_Joystick.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES GPIOmodule)
component_compile_options(-std=gnu++17)
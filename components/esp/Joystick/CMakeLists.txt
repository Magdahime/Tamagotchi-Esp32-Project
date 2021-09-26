idf_component_register(SRCS "Joystick.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES GpioDriver)
component_compile_options(-std=gnu++17)
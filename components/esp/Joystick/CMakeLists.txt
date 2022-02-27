idf_component_register(SRCS "Joystick.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES GpioDriver
                    PRIV_REQUIRES Game Globals)
component_compile_options(-std=gnu++17)
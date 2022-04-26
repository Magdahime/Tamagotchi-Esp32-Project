idf_component_register(SRCS "EspnowDriver.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES nvs_flash)
component_compile_options(-std=gnu++17)

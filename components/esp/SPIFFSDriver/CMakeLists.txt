idf_component_register(SRCS "SPIFFSDriver.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES spiffs)
component_compile_options(-std=gnu++17)
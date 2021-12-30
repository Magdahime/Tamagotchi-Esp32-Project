idf_component_register(SRCS "EspGLUtils.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES SPIFFSDriver)
component_compile_options(-std=gnu++17)

idf_component_register(SRCS "Serializer.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES SPIFFSDriver)
                    component_compile_options(-std=gnu++17)
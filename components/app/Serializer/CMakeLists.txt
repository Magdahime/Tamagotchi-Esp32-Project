idf_component_register(SRCS "Serializer.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES SPIFFSDriver Pet)
                    component_compile_options(-std=gnu++17)
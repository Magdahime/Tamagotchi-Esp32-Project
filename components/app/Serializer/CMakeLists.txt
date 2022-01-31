idf_component_register(SRCS "Serializer.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES SPIFFSDriver Pet Globals)
                    component_compile_options(-std=gnu++17)
idf_component_register(SRCS "testPetGenerator.cc"
                       REQUIRES unity test_utils EspGL SPIFFSDriver PetGenerator BitmapLoader)
                       component_compile_options(-std=gnu++17)
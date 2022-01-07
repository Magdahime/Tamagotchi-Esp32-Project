idf_component_register(SRCS "testBitmapLoader.cc"
                       REQUIRES unity test_utils SPIFFSDriver EspGL)
                       component_compile_options(-std=gnu++17)
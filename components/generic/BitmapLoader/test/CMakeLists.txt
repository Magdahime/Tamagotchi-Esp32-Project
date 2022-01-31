idf_component_register(SRCS "testBitmapLoader.cc"
                       REQUIRES unity test_utils Globals EspGL)
                       component_compile_options(-std=gnu++17)
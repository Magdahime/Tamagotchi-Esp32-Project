idf_component_register(SRCS "ColourProvider.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES  BitmapLoader EspGL SPIFFSDriver Pet)
                    component_compile_options(-std=gnu++17)
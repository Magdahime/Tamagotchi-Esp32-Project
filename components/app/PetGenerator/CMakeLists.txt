idf_component_register(SRCS "ColourProvider.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES  BitmapLoader EspGL Pet Globals)
                    component_compile_options(-std=gnu++17)
idf_component_register(SRCS "SPIDriver.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES RecyclingContainer)
component_compile_options(-std=gnu++17)
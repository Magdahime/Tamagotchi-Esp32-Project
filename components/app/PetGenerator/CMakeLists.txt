idf_component_register(SRCS "PetGenerator.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES Pet EspGL)
component_compile_options(-std=gnu++17)

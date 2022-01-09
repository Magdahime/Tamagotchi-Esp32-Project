idf_component_register(SRCS "testPet.cc"
                       REQUIRES unity test_utils EspGL PetGenerator)
component_compile_options(-std=gnu++17)
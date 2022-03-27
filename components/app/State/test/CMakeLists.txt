idf_component_register(SRCS "testStateUtils.cc" 
                       REQUIRES unity test_utils EspGL Globals Game)
component_compile_options(-std=gnu++17)
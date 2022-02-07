idf_component_register(SRCS "StartState.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES Event Globals Serializer PetGenerator Pet)
component_compile_options(-std=gnu++17)
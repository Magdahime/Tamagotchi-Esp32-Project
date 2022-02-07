idf_component_register(SRCS "StartState.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES Event PetGenerator Serializer Game Globals)
component_compile_options(-std=gnu++17)
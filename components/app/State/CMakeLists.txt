idf_component_register(SRCS "StartState.cc" "EndState.cc" "State.cc" "MainMenuState.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES Event PetGenerator Serializer Game Globals)
component_compile_options(-std=gnu++17)
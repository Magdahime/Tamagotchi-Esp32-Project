idf_component_register(SRCS "StartState.cc" "EndState.cc" "State.cc" "MainMenuState.cc" "StateUtils.cc" "MiniGameState.cc" "NormalPlayerState.cc" "GameHostState.cc" 
                    INCLUDE_DIRS "include"
                    REQUIRES Event PetGenerator Serializer Game Globals Gomoku GomokuNetworking ColourProvider)
component_compile_options(-std=gnu++17)
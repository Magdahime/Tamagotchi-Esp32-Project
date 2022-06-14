idf_component_register(SRCS "GomokuNetworking.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES MessageQueue Gomoku Game Globals nvs_flash ColourProvider)
component_compile_options(-std=gnu++17)